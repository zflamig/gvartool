#include "utils/fileBuf/fileBuf.h"
#include "utils/options/options.h"
#include "include/FrameList.h"

extern FileBuf *(*OutFile)[INSTRUMENTS];
extern Options Opt;

FrameListItem::FrameListItem(void):
  FrameNumber(-1), Instr(-1), File(0)
{
} // end FrameListItem default constructor

FrameListItem::FrameListItem(int instr):
  FrameNumber(-1), Instr(imager)
{
  Instr = instr;
  File = new FileBuf *[Opt.sectors(Instr)];
  for (int f=0; f<Opt.sectors(Instr); f++)
  {
    File[f] = NULL;
  }
} // end FrameListItem constructor




FrameListItem::~FrameListItem(void)
{
  if (File)
  {
    Close();
    delete [] File;
    File = 0;
  }
} // end FrameListItem destructor




void FrameListItem::Save(int instr, int frame_number)
{
  int some_outfile_not_null = 0;
  Instr = instr;

  // if File doesn't exit, make it and set to null 
  if (!File)
  {
    File = new FileBuf *[Opt.sectors(Instr)];
    for (int f=0; f<Opt.sectors(Instr); f++)
      File[f] = NULL;
  }

  // if any OutFile not null ...
  for (int f=0; f<Opt.sectors(Instr); f++)
    some_outfile_not_null += (OutFile[f][Instr]!=0);

  // save them to File
  if (some_outfile_not_null)
  {
    FrameNumber = frame_number;
    for (int f=0; f<Opt.sectors(Instr); f++)
      File[f] = OutFile[f][Instr];
  }
//Dump();
} // end FrameListItem::Save




void FrameListItem::Restore(void)
{
  for (int f=0; f<Opt.sectors(Instr); f++)
    OutFile[f][Instr] = File[f];
} // end FrameListItem::Restore




void FrameListItem::Close(void)
{
  if (Instr>=0)
  {
    int f;
    for (f=0; f<Opt.sectors(Instr); f++)
    {
      if (File[f])
      {
//std::cout << "****> "<<(unsigned long)File[f]<<" "<<(unsigned long)OutFile[f][Instr]<<"\n"<<std::flush;
        if (File[f]==OutFile[f][Instr] && OutFile[f][Instr]!=0)
          OutFile[f][Instr] = 0;
//std::cout << "****> "<<(unsigned long)File[f]<<" "<<(unsigned long)OutFile[f][Instr]<<"\n"<<std::flush;
        delete File[f]; //??? do NOT delete files here, let OutFile do it
        File[f] = 0;
//std::cout << "****> "<<(unsigned long)File[f]<<" "<<(unsigned long)OutFile[f][Instr]<<"\n"<<std::flush;
      }
    }
  }
} // end FrameListItem::Close





FrameList::FrameList(void):
  frame_list_item(0)
{
  // 2 instruments (imager and sounder)
  //  and 4 possible priority levels for each instrument
  int i;
  frame_list_item = new FrameListItem*[2];
  for (i=imager; i<=sounder; i++)
    frame_list_item[i] = new FrameListItem[4];
} // end FrameList constructor





FrameList::~FrameList(void)
{
  if (frame_list_item)
  {
    for (int i=imager; i<=sounder; i++)
      delete [] frame_list_item[i];
    delete [] frame_list_item;
  }
} // end FrameList destructor





void FrameList::Save(int instr, unsigned char frame_number, int priority)
{
  if (frame_list_item[instr][priority].FrameNumber != (int)frame_number)
  {
    //std::cout << "save    " << (int)frame_number << " (" << priority << ")\n" << std::flush;
    frame_list_item[instr][priority].Save(instr, (int)frame_number);
  }
} // end FrameList::Save





int FrameList::Check(int instr, unsigned char frame_number, int priority)
{
  int retval = 0;
  //Dump();
  if (frame_list_item[instr][priority].FrameNumber==(int)frame_number)
  {
    // restore old files for this priority
    //std::cout << "restore " << (int)frame_number << " (" << priority << ")\n" << std::flush;
    frame_list_item[instr][priority].Restore();
    // do not open new files
  }
  else
  {
    // close old files for this priority
    if (frame_list_item[instr][priority].FrameNumber != -1)
    {
      std::cout << "close frame number " << frame_list_item[instr][priority].FrameNumber << ", priority = " << priority << " \n" << std::flush;
    }
    frame_list_item[instr][priority].Close();
    // prepare to open new files (redundant)
    //for (int f=0; f<Opt.sectors(instr); f++)
    //{
    //  OutFile[f][instr] = 0;
    //}

    retval = 1;
  }
  return retval;
} // end FrameList::Check




void FrameList::Dump(void)
{
  for (int i=imager; i<=sounder; i++)
  {
    for (int p=0; p<4; p++)
    {
      std::cout << "FrameList[" << i << "][" << p << "]:\n";
      frame_list_item[i][p].Dump();
    }
  }
} // end FrameList::Dump

void FrameListItem::Dump(void)
{
  std::cout << "  FrameNumber ("<<FrameNumber<<") Instr ("<<Instr<<")\n";
  std::cout << "  Files: "<<(unsigned long)File<<"\n";
  for (int f=0; File!=0 && f<Opt.sectors(Instr); f++)
  {
    std::cout << "    " << (unsigned long)File[f]<<"\n";
  }
  std::cout.flush();
} // end FrameListItem::Dump
