#ifndef _FRAMELIST_H_
#define _FRAMELIST_H_

// 4 possible priorities (probably only 2 are really used)
// 2 instruments
// list[priority][instrument] has FrameNumber and FileBuf[sectors]
//
// The number of 'sectors' as relates to FileBuf is the number of separate 
// '-imager' or '-sounder' statements in the options file or on the command 
// line. (ImagerSectors or SounderSectors respectively)
//
// Because this list will keep multiple sets of files open when processing
// priority interrupts, it is possible to run out of file handles and crash
// when too many 'sectors' are specified.  Empirically, it seems the limit
// without priority processing is about 30 'sectors'.  This number is reduced
// by one-half to one-quarter with priority processing.

struct FrameListItem
{
  int FrameNumber;
  int Instr;
  FileBuf *(*File); // FileBuf *File[Nsectors]

  FrameListItem(void);
  FrameListItem(int instr);
  ~FrameListItem(void);
  void Save(int instr, int frame_number);
  void Restore(void);
  void Close(void);
  void Dump(void);
};

class FrameList
{
  struct FrameListItem **frame_list_item; // list[Ninstruments][Npriorities]
public:
  FrameList(void);
  ~FrameList(void);
  void Save(int instr, unsigned char frame_number, int priority);
  int Check(int instr, unsigned char frame_number, int priority);
  void Dump(void);
};

#endif // FrameList.h
