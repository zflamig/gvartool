#ifdef __GNUG__
#pragma interface 
#endif

#ifndef FILEBUF_H
#define FILEBUF_H

#include <string.h>
//#include <stream.h>
#include <iostream>
using std::ostream;


#include "include/gvar.h"
#include "include/types.h"

#define CLEAN 0
#define KEEP  1
#define DIRTY 2


class FileBuf{ 
protected:

  char isOpen;
  char* GlobalComment; 
  char* FnameStub;
  
  int Compress;
  int LinkLatest;

  unsigned int Channels; 
  char* *ChannelComment;  
  WORD_TYPE *ChannelWordType; 
  unsigned int *ChannelXSize;
  unsigned int *ChannelYSize;
  int *ChannelXStart;
  int *ChannelYStart;
  double *ChannelXStride;
  double *ChannelYStride;
  double *ChannelMax;
  double *ChannelMin;
  char* *ChannelDirectory;

  unsigned int BufLines;
  char* *Buf; 

  int *BufPageCounter; 
  int* *BufLineStatus; 

  // methods
  void* DataPtr(int channel, int line, int pixel =0); 

  
  double xStride(int channel);
  double yStride(int channel);

  int bufLineStatus(int channel, int line=0);
  void setBufLineStatus(int channel, int line, int status);
  int imageLine(int channel, int bufline=0);

  void bufZeroDirty(int channel) ; 
  void bufZeroClean(int channel) ; 
  void bufMakeSpace(int channel);		
  void bufWrite(int channel);
  void bufFlush(int channel) ; 

  int wordSize(int channel);
  
  char* tmpName(char* name);
  void MakeLink(int channel, char * Extn = NULL );

public:
  FileBuf(int bufLines = 1);
  void setBufLines(int bufLines);

  int xStart(int channel);
  int yStart(int channel);

  unsigned int xSize(int channel);
  unsigned int ySize(int channel);
  
  void setChannelSizes(int channel, unsigned int ys, unsigned int xs);
  void setChannelOffsets(int channel, int ys, int xs);
  void setChannelStrides(int channel, double ys, double xs);

  void setFnameStub(char * text);
  int setCompress(int compress); 
  int & linkLatest();

  void setChannels(int channels=1);

  void setComment(char * text, int channel = -1);
  void setChannelWordType(int channel=0, WORD_TYPE wordType= Ushort);
  void setChannelMinMax(int channel, double min = 0, double max = 1);
  void setChannelDirectory(int channel, char * dir);

  int inBounds(int channel, int y, int x,  int xs=1 );
  int lineClean(int channel, int y, int x, int xs=1);

  void writeData
    (int channel, int line, int pixel, void * data, int pixels=1);

  
  void print(ostream & out);
  
  virtual ~FileBuf();
  virtual int Open(int channel);
  virtual int DiskWrite(int channel);
  virtual int Close(int channel);
  virtual char* FileName(int channel, char * Extn = NULL );
};

inline FileBuf::FileBuf(int bufLines)
{ BufLines = bufLines; Channels = 1; isOpen = 0;}

inline void FileBuf::setBufLines(int bufLines){
  BufLines = bufLines; }

inline void FileBuf::setChannelSizes
(int channel, unsigned int ys, unsigned int xs)
{ ChannelYSize[channel] = ys ; ChannelXSize[channel] = xs; }

inline void FileBuf::setChannelStrides
(int channel, double ys, double xs)
{ ChannelYStride[channel] = ys ; ChannelXStride[channel] = xs; }
     
inline void FileBuf::setChannelOffsets
(int channel, int ys, int xs)
{ ChannelYStart[channel] = ys; ChannelXStart[channel] = xs; }

inline int FileBuf::wordSize(int channel)
{return WordSize[ChannelWordType[channel]];}

inline void FileBuf::setBufLineStatus
(int channel, int line,int status)
{BufLineStatus[channel][line%BufLines] = status; }

inline int FileBuf::bufLineStatus(int channel, int line)
{return (BufLineStatus[channel])[line%BufLines]; }

inline int FileBuf::setCompress(int compress)
{return(Compress=compress); }

inline int & FileBuf::linkLatest()
{return LinkLatest; }

inline int FileBuf::xStart(int channel)
{return ChannelXStart[channel]; }

inline int FileBuf::yStart(int channel)
{return ChannelYStart[channel]; }

inline unsigned int FileBuf::xSize(int channel)
{return ChannelXSize[channel]; }

inline unsigned int FileBuf::ySize(int channel)
{return ChannelYSize[channel]; }

inline double FileBuf::xStride(int channel)
{return ChannelXStride[channel]; }

inline double FileBuf::yStride(int channel)
{return ChannelYStride[channel]; }

inline void FileBuf::setChannelWordType(int channel, WORD_TYPE wordType)
{ ChannelWordType[channel] = wordType ;} 

inline int FileBuf::inBounds
(int channel,int y,int x,int xs )  {
  return
    ((x + xs >= xStart(channel)) &&
     (y      >= yStart(channel)) &&  
     (x      <= (int)(xStart(channel) +xSize(channel)) ) &&
     (y      <= (int)(yStart(channel) +ySize(channel)) ) 
     );
}

inline void FileBuf::setChannelMinMax
(int channel,double min, double max )
{ ChannelMin[channel] = min; ChannelMax[channel] = max; }

inline int FileBuf::imageLine(int channel, int bufline)
{ return(BufPageCounter[channel]*BufLines + bufline );}

inline void* FileBuf::DataPtr(int channel, int y, int x )
{ return( (char*) Buf[channel]
	 +(y*xSize(channel)+x)
	 *wordSize(channel) );} 


//inline int FileBuf::lineClean
//(int channel, int y, int x, int xs){
//  return((inBounds(channel, y, x, xs) ) && 
//	 (bufLineStatus(channel, y) != KEEP || 
//	  y/BufLines != BufPageCounter[channel])
//	 );
//}

#endif // FileBuf_H
     
