#include "imc.h"
#include "utils/bitfield/bitfield.h"

static char * SounderStatus[]= { 
	"\t0x0 Spacelook in progress", // 1
	"\t0x1 ECAL in progress", // 2
	"\t0x2 Blackbody calibration in progress", //3 
	"\t0x3 Normal frame in progress", // 4
	"\t0x4 Priority 1 frame in progress" , // 5
	"\t0x5 Priority 2 frame in progress", // 6
	"\t0x6 East-to-west scan", // 7
	"\t0x7 South-to-north scan", // 8 
	"\t0x8 IMC active", // 9
	"\t0x9 Dwell mode = 4", // 10
	"\t0xa Dwell mode = 2", // 11
	"\t0xb Dwell mode = 1", // 12
	"\t0xc N/S step mode = double", // 13
	"\t0xd Side 2 electronics active" , // 14
	"\t0xe Star sense in progress" , //15
	"\t0xe Instrument slew in progress" // 16
	};

static char * ImagerStatus[] = { 
	"\t0x0 Spacelook in progress", //1
	"\t0x1 Preclamp scan in progress",//2
	"\t0x2 Blackbody calibration in progress", //3
	"\t0x3 Normal frame in progress", //4
	"\t0x4 Priority 1 frame in progress" , // 5
	"\t0x5 Priority 2 frame in progress", // 6
	"\t0x6 East-to-west scan", // 7
	"\t0x7 South-to-north scan", // 8 
	"\t0x8 IMC active", // 9
	"\t0x9 Scan clamp mode active", //10
	"\t0xa Fast space clamp mode active (9.2 sec)",//11
	"\t0xb Slow space clamp mode active (36.6sec)",//12
	"\t0xc Reversal data", //13
	"\t0xd Side 2 electronics active",//14
	"\t0xe Star sense in progress",//15
	"\t0xf Instrument slew in progress"//16
	};

void Imc::print(std::ostream & out = std::cout ) { 

	out << "\n";
	Bitfield bits(status);
	for ( int i = 0; i < 16 ; i++)
		out << (int)bits.status(i) <<"\t"; 

	out <<  EWcycles; 
	out << "\t" << EWincrement; 
	out << "\t" << NScycles; 
	out << "\t" << NSincrement; 

	out << "\t" << EWcompensation; 
	out << "\t" << EWservoError; 
	
	out << "\t" << NScompensation; 
	out << "\t" << NSservoError; 
}

void Imc::ImagerText( std::ostream & out = std::cout ){
	out << "Imager Instrument Compensation and Servo Errors"; 
	out << "\nIncrements per cycle : 6135";
	out << "\nImager Status Bit Code :"; 
		for ( int i = 0; i < 16 ; i++)
		out << "\n" << ImagerStatus[i];
	out << "\n0x0\t0x1\t0x2\t0x3\t0x4\t0x5\t0x6\t0x7\t0x8\t0x9\t0xa\t0xb\t0xc\t0xd\t0xe\t0xf\tEWcycles\tEWincrements\t" 
		<< "NScycles\tNSincrements\tEWcompensation\t" 
			<<"EWservoErrors\tNScompensation\tNSservoErrors" ; 

}

void Imc::SounderText( std::ostream & out = std::cout ){
	out << "Sounder Instrument Compensation and Servo Errors"; 
	out << "\nIncrements per cycle : 2804" ;
	out << "\nSounder Status Bit Code :";
	for ( int i = 0; i < 16 ; i++)
		out << "\n" << SounderStatus[i];
	out << "\n0x0\t0x1\t0x2\t0x3\t0x4\t0x5\t0x6\t0x7\t0x8\t0x9\t0xa\t0xb\t0xc\t0xd\t0xe\t0xf\tEWcycles\tEWincrements\t" 
		<< "NScycles\tNSincrements\tEWcompensation\t" 
			<<"EWservoErrors\tNScompensation\tNSservoErrors" ; 

}
