
#include <string.h>

#include "utils/crcCheck/crcCheck.h"
#include "gvarHeader.h"

#define WORD_COUNT_MIN 8040
#define WORD_COUNT_MAX 42008
#define WORD_SIZE_MAX 11
#define BLOCK_ID_MAX 12

void GvarHeader::copyHeader(char *hdr0) 
{ memcpy(&Hdr, hdr0, N_HEADER_BYTES); } 


int GvarHeader::isValidHeader() { 

  return((wordSize() != 0 ) 
	 && (wordCount() != 0 )  
	 && (blockCount() != 0 )    
	 && (blockId() < BLOCK_ID_MAX) 
	 && (wordSize() < WORD_SIZE_MAX) 
	 && (wordCount() < WORD_COUNT_MAX)  
	 && (dataValid() == 1)
	 
	 );
}	


uint16 GvarHeader::calc_crc() { 

  return( 65535 - crcCheck( &Hdr, N_HEADER_BYTES-2) );

}	


int GvarHeader::crc_is_good() { 

  return( calc_crc() == crc() );

}	


void GvarHeader::print( std::ostream & out ) { 

  out << "\n\t Block Header "; 
  out << "\n Block Id " << blockId(); 
  out << "\n Word size " << wordSize() ; 
  out << "\n Word count " << wordCount() ; 
  out << "\n Product Id " << productId() ; 
  //	out << "\n\t" <<  ProductIDText[productID() ] ; 
  out << "\n Repeat flag " << repeatFlag() ; 
  out << "\n Version number " << versionNumber() ; 
  out << "\n Data Valid flag " << dataValid() ; 
  out << "\n ASCII / BIN data flag " << asciiBin() ; 
  out << "\n Range word " << rangeWord() ; 
  out << "\n Block count " << blockCount() ;
  out << "\n crc_is_good " << crc_is_good() ;
  out << "\n";
}


void GvarHeader::printHeader(std::ostream & out ){
  out << blockId() << "\t";
  out << wordSize() << "\t";
  out << wordCount() << "\t\t";
  out << productId() << "\t";
  out << repeatFlag() << "\t";
  out << versionNumber() << "\t";
  out << dataValid() << "\t";
  out << asciiBin() << "\t";
  out << rangeWord() << "\t";
  out << blockCount()<< "\t" ;
  //  for (int i=0;i<8;i++) out << Hdr.SPS_time[i] << "\t" ;
  out  << crc_is_good() << "\t" ;
  //out  << "\n" ;
  out.flush();
} 


void GvarHeader::printDframe(std::ostream & out ){
  out << blockCount()<< "\t" ;
  out << dataValid() << "\t";
  out << blockId() << "\t";
  out << productId() << "\t";
  out.flush();
} 


void GvarHeader::headerText(std::ostream & out ){

  out << "BlockId\t";
  out << "WordSz\t";
  out << "WordCount\t";
  out << "ProdId\t";
  out << "Repeat\t";
  out << "Version\t";
  out << "Valid\t";
  out << "AsciiBin\t";
  out << "RangeWord\t";
  out << "BlockCount\t";
  out << "crc_is_good" <<"\t";
}
