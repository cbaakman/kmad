#ifndef TXTPROC_H
#define TXTPROC_H

#include "Sequences.h"

#include <iostream>
#include <string>
#include <vector>
class FeaturesProfile;
class Sequences;
namespace txtProc{
	double convertStringToDouble(std::string&);
	std::string charToString(char);
	std::string charToString(char,char);
	std::istream& safeGetline(std::istream&, std::string&);
	std::vector<std::string> split(const std::string&, char);
	bool acceptedChar(char);
	Sequences read_fasta(std::string, int, std::vector<std::string>*,
                       std::vector<double>*);
  //std::vector<std::vector<std::vector<std::string> > > read_fasta(std::string, int, std::vector<std::string>*,
  //                     std::vector<double>*);
	void writeAlignmentToFile(std::vector<std::string>&, 
                            std::vector<std::string>&,
                            std::string);
	void writeAlignmentWithoutCodeToFile(std::vector<std::string>&, 
                                       std::vector<std::string>&, 
                                       std::string, int);
	void process_conf_file(std::string, FeaturesProfile&, Sequences&);
	std::vector<int> unfold(std::string, std::vector<std::string>&);
}

#endif /* TXTPROC_H */
