
#include "sadId.h"

void SadId::print( ostream & out ) 
{ 
  out << (int)Spcid << "\t"; 
  out << (int)ground_system_id << "\t"; 
  out << dataId() << "\t"; 
  out << (int)Fbf << "\t"; 
  out << (int)Lbf << "\t"; 
  out << blockCount() << "\t";
  out << nrec() ;

}


void SadId::printHeader( ostream & out ){
  out << "Spcid\t";
  out << "Spsid\t";
  out << "DataId\t";
  out << "FirstBlockFlag\t";
  out << "LastBlockFlag\t";
  out << "BlockCount\t";
  out << "RecordCount";
  out.flush();
}

