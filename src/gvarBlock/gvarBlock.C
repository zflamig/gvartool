#include <string.h> 
#include "gvarBlock.h"

GvarBlock::GvarBlock(void/*int bufSiz*/)
{
  Trash = 0; 
  BytesRead = 1; 
  // no way to set buffer size in new ifstream
  //InBuf = new char [bufSiz];
  //Ifs.setbuf(InBuf, bufSiz);
  Data = new short [MAX_BLOCK_SZ]; 
} 

void GvarBlock::open(char * fileName)
{
  struct stat Stat;
  close();
  Ifs.open(fileName) ;
  stat(fileName, &Stat);
  fileSize = (int) Stat.st_size;
  Trash = 0;
  BlockCounter = 0;
  BytesRead = 0;
}


std::istream & GvarBlock::FindHeader(){ 

  int Found = 0 ; 
  char next_byte=0; 
  int match=0;
  int i = 0 ; 

  char header_array[N_HEADER_BYTES];
  char ordered_header[N_HEADER_BYTES];

  Ifs.read(header_array, N_HEADER_BYTES );

  while(Ifs && !Found ) {
    
    // look for 2 matching 30 byte strings

    for( match = 0;         match < N_HEADER_BYTES && Ifs.get(next_byte) ;        ++i ){ 
      if( next_byte == header_array[i%N_HEADER_BYTES]) {
         match++ ;
      }
      else { 
         header_array[i%N_HEADER_BYTES] = next_byte; 
         match = 0;
      } 
    }

    // when we get here, match == N_HEADER_BYTES
    //     and file read position is at start of 3rd header

    if(header_array[(i+1)%N_HEADER_BYTES] != 0 ) {       // first entry cannot be zero 

      // shift the array so that 0th entry corresponds to header[0]
      for( int j = 0; j < N_HEADER_BYTES; j++)
        ordered_header[j] = header_array[(i+j)%N_HEADER_BYTES];
      
      // read in 3rd potential header
      Ifs.read(header_array, N_HEADER_BYTES); 
      i += N_HEADER_BYTES;

      // if number of mismatched bytes is zero
      if (memcmp( header_array, ordered_header , N_HEADER_BYTES) == 0){
        GvarHeader::copyHeader(header_array); 
        Found = GvarHeader::isValidHeader();
        //Found = GvarHeader::crc_is_good();
        if (Found) { i -= 2*N_HEADER_BYTES; BlockCounter++; }
      }
    }
  }

  trash(i);
  return(Ifs);
}

std::istream & GvarBlock::better_FindHeader(){ 

  int Found = 0 ; 
  char next_byte=0; 
  int i; 
  int j; 
  int ii; 
  int jj; 
  int offset; 
  int good_offset; 
  int go_back_bytes; 
  int crc_list[5];
  int num_match_bytes[5]; 
  int crc_total;
  int a,b,c;
  int junked_bytes=0;

  char header_array[5*N_HEADER_BYTES];

  // headers
  GvarHeader hdr1;
  GvarHeader hdr2;
  GvarHeader hdr3;
  GvarHeader hdr4;
  GvarHeader hdr5;

  // read in 5 sequential potential headers
  Ifs.read(header_array, 5*N_HEADER_BYTES );

  while(Ifs && !Found ) {
    
     crc_total=0;
     i = 0;
     while( i<5 ) {
        GvarHeader::copyHeader(&(header_array[i*N_HEADER_BYTES])); 
        crc_list[i] = GvarHeader::crc_is_good();
        crc_total += crc_list[i];
        i++;
     }

     // if (crc_total < 3) {
       //std::cout << "\n\n crc_list " << crc_list[0] << " " << crc_list[1]<< " ";
       //std::cout << crc_list[2]<< " " << crc_list[3]<< " " << crc_list[4]      << " \n" << std::flush;

       //for (i=0;i<5;i++) { 
       // for (j=0; j<N_HEADER_BYTES; j++) std::cout << (int)header_array[i*N_HEADER_BYTES+j]  << " " ;
	 //std::cout << "\n" << std::flush;
       //	}
       //std::cout << "\n" << std::flush;
     // }

     good_offset = -1;
     offset = -1;
     if (crc_total >= 3) {
       if      (crc_list[0] && crc_list[1] && crc_list[2]) {offset = 0; good_offset = 0;} // normal noise-free case
       else if (crc_list[1] && crc_list[2] && crc_list[3]) {offset = 1; good_offset = 1;}
       else if (crc_list[2] && crc_list[3] && crc_list[4]) {offset = 2; good_offset = 2;}
     } 
     else if (crc_total == 2) {

       if      (crc_list[0] &&                crc_list[2]) {offset = 0; good_offset = 0;}
       else if (crc_list[0] && crc_list[1]               ) {offset = 0; good_offset = 0;}
       else if (crc_list[1] &&                crc_list[3]) {offset = 1; good_offset = 1;}
       else if (crc_list[2] &&                crc_list[4]) {offset = 2; good_offset = 2;}
       else if (crc_list[1] && crc_list[2]               ) {

 	  // is [0,1,2] or [1,2,3] a more likely header set?
	  j=1; // header with a good crc
          jj=j*N_HEADER_BYTES;

          for (i=0; i<5; i++) {
 	     num_match_bytes[i] = 0;
	     for (ii=i*N_HEADER_BYTES;  ii<(i+1)*N_HEADER_BYTES;  ii++,jj++) { 
                num_match_bytes[i] += (header_array[ii] == header_array[jj]);
             }
	  }

          if (num_match_bytes[0] >= num_match_bytes[3]) {offset = 0; good_offset = 1;}
          else                                          {offset = 1; good_offset = 1;}


       }
       else if (crc_list[2] && crc_list[3]               ) {
 	  // is [1,2,3] or [2,3,4] a more likely header set?
	  j=2; // header with a good crc
          jj=j*N_HEADER_BYTES;

          for (i=0; i<5; i++) {
 	     num_match_bytes[i] = 0;
	     for (ii=i*N_HEADER_BYTES;  ii<(i+1)*N_HEADER_BYTES;  ii++,jj++) { 
                num_match_bytes[i] += (header_array[ii] == header_array[jj]);
             }
	  }

          if (num_match_bytes[1] >= num_match_bytes[4]) {offset = 1; good_offset = 2;}
          else                                          {offset = 2; good_offset = 2;}

       }
     }
     else if (crc_total == 1) {

       // there is the only good crc, 
       // only test for 0,1,2 (3,4 may indicate a header still to shift in to the array)
       // check values seem ok? want at most x errors per header, should x be 2, 3 ...???
       if (crc_list[0] ) {

	  j=0; // header with a good crc
          jj=j*N_HEADER_BYTES;

          for (i=0; i<5; i++) {
 	     num_match_bytes[i] = 0;
	     for (ii=i*N_HEADER_BYTES;  ii<(i+1)*N_HEADER_BYTES;  ii++,jj++) { 
                num_match_bytes[i] += (header_array[ii] == header_array[jj]);
             }
	  }

 	  //  [0,1,2] is the only possibility, do enough bytes match?
          if (num_match_bytes[1] >= 20 && num_match_bytes[2] >= 20) {offset = 0; good_offset = 0;}
       }
       else if (crc_list[1] ) {

	  j=1; // header with a good crc
          jj=j*N_HEADER_BYTES;

          for (i=0; i<5; i++) {
 	     num_match_bytes[i] = 0;
	     for (ii=i*N_HEADER_BYTES;  ii<(i+1)*N_HEADER_BYTES;  ii++,jj++) { 
                num_match_bytes[i] += (header_array[ii] == header_array[jj]);
             }
	  }

 	  //  [0,1,2] or [1,2,3] are possible header sets
          if (num_match_bytes[0] >= num_match_bytes[3]) {offset = 0; good_offset = 1;}
          else                                          {offset = 1; good_offset = 1;}
       }
       else if (crc_list[2] ) {

	  j=2; // header with a good crc
          jj=j*N_HEADER_BYTES;

          for (i=0; i<5; i++) {
 	     num_match_bytes[i] = 0;
	     for (ii=i*N_HEADER_BYTES;  ii<(i+1)*N_HEADER_BYTES;  ii++,jj++) { 
                num_match_bytes[i] += (header_array[ii] == header_array[jj]);
             }
	  }

 	  //  [0,1,2], [1,2,3], [2,3,4] are possible header sets
          a = num_match_bytes[0] + num_match_bytes[1];
          b = num_match_bytes[1] + num_match_bytes[3];
          c = num_match_bytes[3] + num_match_bytes[4];

          if       (a >= b && a >=c )                     {offset = 0; good_offset = 2;}
          else if  (b >= a && b >=c )                     {offset = 1; good_offset = 2;}
          else                                            {offset = 2; good_offset = 2;}
       }

     }



     // == header not found
     if (offset == -1)  {
       // shift array by 1 byte, read in 1 byte and go back to start of while loop



       //std::cout << " 0 header_array " << (int)header_array[0] << " " << (int)header_array[1]<< " " << (int)header_array[2]<< " " << (int)header_array[3]<< " " << (int)header_array[5*N_HEADER_BYTES-1]      << " \n" << std::flush;

       for (j=0; j<5*N_HEADER_BYTES-1; j++) { header_array[j] = header_array[j+1]; }
       //std::cout << " 1 header_array " << (int)header_array[0] << " " << (int)header_array[1]<< " " << (int)header_array[2]<< " " << (int)header_array[3]<< " " << (int)header_array[5*N_HEADER_BYTES-1]      << " \n" << std::flush;

       Ifs.get(next_byte) ;
       junked_bytes += 1;
       //std::cout << " read a byte  \n" << std::flush;
          
       header_array[5*N_HEADER_BYTES-1] = next_byte;
       //std::cout << " 2 header_array " << (int)header_array[0] << " " << (int)header_array[1]<< " " << (int)header_array[2]<< " " << (int)header_array[3]<< " " << (int)header_array[5*N_HEADER_BYTES-1]      << " \n" << std::flush;

     }
     else { // header was found

       // if (offset >= 1  &&  crc_total == 3 ) {
       //    std::cout << " crc_list " << crc_list[0] << " " << crc_list[1]<< " " << crc_list[2]<< " " << crc_list[3]<< " " << crc_list[4]      << " \n" << std::flush;
       //   for (i=0;i<5;i++) { 
       //	     for (j=0; j<N_HEADER_BYTES; j++) std::cout << (int)header_array[i*N_HEADER_BYTES+j]  << " " ;
       //       std::cout << "\n" << std::flush;
       //    }
       // }

       // if (  crc_total < 3 ) {
       if (  crc_total < 0 ) { // debug statement is never true ... edit for debugging
            std::cout << " offset " <<  offset << " good offset " <<  good_offset   << "\n" << std::flush;

            hdr1.copyHeader(&(header_array[0*N_HEADER_BYTES])); 
            hdr2.copyHeader(&(header_array[1*N_HEADER_BYTES])); 
            hdr3.copyHeader(&(header_array[2*N_HEADER_BYTES])); 
            hdr4.copyHeader(&(header_array[3*N_HEADER_BYTES])); 
            hdr5.copyHeader(&(header_array[4*N_HEADER_BYTES])); 

            hdr1.printHeader(std::cout); 
            hdr2.printHeader(std::cout); 
            hdr3.printHeader(std::cout); 
            hdr4.printHeader(std::cout); 
            hdr5.printHeader(std::cout); 
            std::cout << "\n\n" << std::flush;
        }

        GvarHeader::copyHeader(&(header_array[good_offset*N_HEADER_BYTES])); 

        go_back_bytes = 0 - (2 - offset) * N_HEADER_BYTES;

        if ( offset != 0) {
	   std::cout << "************************ found a block " << Found << " offset " << offset;
           std::cout << " go back " << go_back_bytes << "\n" << std::flush;
	}

        Ifs.seekg (go_back_bytes, std::ios::cur);

        Found = 1;
        BlockCounter++; 
     } // end header was found section

  } // end while loop


  trash(junked_bytes);
  return(Ifs);
}




uint16 GvarBlock::getBlock()
{ 
  //return ( FindHeader()  && GetData() );
  return ( better_FindHeader()  && GetData() );
} 

//1 is bad crc, 0 is good crc
int GvarBlock::crcBlock()
{ 
  return (  crcCheck(data(),  (int)GvarHeader::nBytes() ) != MAGIC_CRC  );
} 

std::istream & GvarBlock::GetData()
{
return Ifs.read( (char *) Data,  (int)GvarHeader::nBytes() ); 
}


GvarBlock::~GvarBlock()  
{ /*delete InBuf;*/ delete (char *)Data; }

void * GvarBlock::data()
{ return Data ; } 

void GvarBlock::close() 
{
  if (Ifs.is_open())
  {
    getBytesRead();
    Ifs.close();
  }
  Ifs.clear();
}

int GvarBlock::trash(int n =0 )
{ Trash += n; return((int)Trash);}

int GvarBlock::getTrashBytes() 
{ return (int)Trash ; }

int GvarBlock:: getBytesRead()  
{
  if (Ifs.eof())
    BytesRead = fileSize;
  else
    BytesRead = (int)Ifs.tellg();
  return BytesRead;
}

float GvarBlock::percentToDo() 
{ return ( 100 - 100.0 * getBytesRead()/fileSize ) ; }

int GvarBlock::blockCounter()
{ return BlockCounter; }

uint16 GvarBlock::crc(){ 

#ifdef _MSBF_
  uint16* CRC =   (uint16*)data() + (nBytes()-1)/2;  
  return( *CRC);
#else
  uint16  CRC = *((uint16*)data() + (nBytes()-1)/2);

  CRC = ((CRC&0xff00)>>8) | ((CRC&0x00ff)<<8)  ;

  return CRC;
#endif


}


