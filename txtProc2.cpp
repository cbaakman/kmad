#include "txtProc.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <istream>
#include <algorithm>
#include <iterator>
//function convertStringToInt
int txtProc::convertStringToInt(std::string s){
	int convertedInt;
	std::istringstream iss(s);
	iss >> convertedInt;
	return convertedInt;
}
double txtProc::convertStringToDouble(std::string s){
	double convertedDouble = atof(s.c_str());;
	return convertedDouble;
}
std::vector<std::string> &txtProc::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::string txtProc::gapCode(int length){
	std::string gap = "-";
	for (int i = 1; i < length; i++){
		gap.push_back('A');
	}
	return gap;
}
std::vector<std::string> txtProc::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
/* function processFASTA
reads 'filename' file, returns an array of sequences and their names - [[sequenceName,sequence],...] */
std::vector< std::vector<std::string> > txtProc::processFASTA(std::string filename){
	std::vector< std::vector<std::string> > resultSequences;
	std::string line;
	std::string fastaSymbol = ">";
	std::vector<std::string> newEntry;
	std::ifstream fastafile (filename.c_str());
	int seqNo = -1;
	std::string newSeq = "";
	if (fastafile.is_open()){
		while(fastafile){
			getline(fastafile,line);
			std::string firstChar = line.substr(0,1);
			if (firstChar == fastaSymbol){
				seqNo++;
				resultSequences.push_back(newEntry);
				resultSequences.at(seqNo).push_back(line);
				resultSequences.at(seqNo).push_back(newSeq);
				std::cout << line << std::endl;

			}
			else{
				resultSequences.at(seqNo).at(1)=resultSequences.at(seqNo).at(1).append(line);	
			}
		}
		fastafile.close();
	}
	else{
		std::cout << "Where is the file????";
	}
	return resultSequences;
}
//function processFASTA - reads fasta file with encoded sequence
//writes sequences + seqNames to vector<vector<string>>; motifs' ids to vector<string> and motifs' probs vector<double>
std::vector< std::vector< std::vector<std::string> > > txtProc::processFASTA(std::string filename,int codonLength, std::vector<std::string>* ids, std::vector<double>* probs){
	std::vector< std::vector< std::vector<std::string> > > resultSequences;
	std::string line;
	std::string fastaSymbol = ">";
	std::ifstream fastafile (filename.c_str());
	std::vector<std::string> newName;
	std::vector<std::string> newSequence;
	std::vector< std::vector<std::string> > newEntry;
	//std::vector<std::string> ids;
	//std::vector<double> probs;
	bool sequences = true;
	int seqNo = -1;
	std::string newSeq = "";
	if (fastafile.is_open()){
		while(fastafile){
			getline(fastafile,line);
			std::string firstChar = line.substr(0,1);
			if (line != std::string("## PROBABILITIES")){
				if (sequences && firstChar == fastaSymbol){
					seqNo++;
					resultSequences.push_back(newEntry);
					newName.push_back(line);
					resultSequences.at(seqNo).push_back(newName);
					newName.clear();
					resultSequences.at(seqNo).push_back(newSequence);
				}
				else if (sequences){
					for (int i = 0; i < line.size();i++){
						if (i % codonLength == 0){
							std::string newResidue = "";
							//j for goes through all codon postions of this residue
							std::cout << line.size() << std::endl;
							for (int j = i;j < i + codonLength; j++){
									newResidue += line[j];
							}
							resultSequences.at(seqNo).at(1).push_back(newResidue);
							std::cout << "newResidue: " << newResidue << std::endl;
						}
					}
				}
				//else means we're already in the motifs probs section
				else{
					std::istringstream iss(line);
					std::vector<std::string> motif{std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{}};
					if (motif.size() == 2){
						ids->push_back(motif.at(0));
						probs->push_back(convertStringToDouble(motif.at(1)));
					}
				}
			}
			else{
				sequences = false;
			}
		}
		fastafile.close();
	}
	else{
		std::cout << "Where is the file????";
	}
	return resultSequences;
}
//function writeAlignmentToFile
void txtProc::writeAlignmentToFile(std::vector<std::string> sequences,std::vector< std::vector<std::string> > sequencesWithNames, std::string filename){
	std::stringstream sstr;
	sstr << filename << "_al";
	std::ofstream outputFile(sstr.str().c_str(),std::ios::out);
	for (int i = 0; i < sequences.size() ;i++){
		outputFile << sequencesWithNames.at(i).at(0)<< "\n" << sequences.at(i) << "\n";
	}
}
//function writeAlignmentToFile ENCODED SEQUENCES
void txtProc::writeAlignmentToFile(std::vector<std::string> sequences,std::vector< std::vector< std::vector<std::string> > > sequencesWithNames, std::string filename){
	std::stringstream sstr;
	sstr << filename << "_al";
	std::ofstream outputFile(sstr.str().c_str(),std::ios::out);
	for (int i = 0; i < sequences.size() ;i++){
		outputFile << sequencesWithNames.at(i).at(0).at(0)<< "\n" << sequences.at(i) << "\n";
	}
}
void txtProc::writeAlignmentWithoutCodeToFile(std::vector<std::string> sequences,std::vector< std::vector<std::vector<std::string> > > sequencesWithNames, std::string filename){
	std::stringstream sstr;
	sstr << filename << "_al";
	std::ofstream outputFile(sstr.str().c_str(),std::ios::out);
	for (int i = 0; i < sequences.size() ;i++){
		outputFile << sequencesWithNames.at(i).at(0).at(0)<< "\n";
		std::string sequence="";
		for (int j = 0; j < sequences.at(i).size(); j+=4){
			sequence += sequences.at(i).at(j);
		}
		outputFile << sequence << std::endl;
	}
}
//write to file (2d vector as a 1d vector)
void txtProc::writeVector(std::vector<std::vector<double>> vec, std::string filename){
	std::stringstream sstr;
	sstr << filename;
	std::ofstream outputFile(sstr.str().c_str(),std::ios::out);
	for (int i = 0; i < vec.size() ;i++){
		for (int j = 0; j < vec[0].size(); j++){
			outputFile << vec[i][j] << std::endl;
		}
	}
}
std::string txtProc::charToString(char mychar){
	return std::string(1,mychar);
}
std::istream& txtProc::safeGetline(std::istream& is, std::string& t)
{
	t.clear();
	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();
        for(;;) {
	        int c = sb->sbumpc();
		switch (c) {
			case '\n':
				return is;
			case '\r':
				if(sb->sgetc() == '\n')
				sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if(t.empty())
				setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
		}
	}
}