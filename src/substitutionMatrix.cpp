//SubtitutionMatrix class implementation
#include "substitutionMatrix.h"
#include "Residue.h"
#include "misc.h"
#include "vecUtil.h"

#include<boost/range/numeric.hpp>
#include <iostream>
#include <vector>
typedef std::vector<char> alphaList;
typedef std::vector<sbstMatColumn> sbstMatColumnsList;
 
namespace {
	static const alphabetVec alphabet = {'A','R','N','D','C','Q','E','G',
                                       'H','I','L','K','M','F','P','S',
                                       'T','W','Y','V'};
	//BLOSUM62
	static const sbst_matrix simScores = {{4, -1, -2, -2, 0, -1, -1, 0, -2, -1, 
                                         -1, -1, -1, -2, -1, 1, 0, -3, -2, 0},
                                        {-1, 5, 0, -2, -3, 1, 0, -2, 0, -3, -2,
                                          2, -1, -3, -2, -1, -1, -3, -2, -3},
                                        {-2, 0, 6, 1, -3, 0, 0, 0, 1, -3, -3, 
                                          0, -2, -3, -2, 1, 0, -4, -2, -3},
                                        {-2, -2, 1, 6, -3, 0, 2, -1, -1, -3, 
                                          -4, -1, -3, -3, -1, 0, -1, -4, -3, -3},
                                        {0, -3, -3, -3, 9, -3, -4, -3, -3, -1, 
                                          -1, -3, -1, -2, -3, -1, -1, -2, -2, -1},
                                        {-1, 1, 0, 0, -3, 5, 2, -2, 0, -3, -2, 
                                          1, 0, -3, -1, 0, -1, -2, -1, -2}, 
                                        {-1, 0, 0, 2, -4, 2, 5, -2, 0, -3, -3, 
                                          1, -2, -3, -1, 0, -1, -3, -2, -2}, 
                                        {0, -2, 0, -1, -3, -2, -2, 6, -2, -4, 
                                          -4, -2, -3, -3, -2, 0, -2, -2, -3, -3}, 
                                        {-2, 0, 1, -1, -3, 0, 0, -2, 8, -3, -3, 
                                          -1, -2, -1, -2, -1, -2, -2, 2, -3}, 
                                        {-1, -3, -3, -3, -1, -3, -3, -4, -3, 4, 
                                          2, -3, 1, 0, -3, -2, -1, -3, -1, 3}, 
                                        {-1, -2, -3, -4, -1, -2, -3, -4, -3, 2, 
                                          4, -2, 2, 0, -3, -2, -1, -2, -1, 1}, 
                                        {-1, 2, 0, -1, -3, 1, 1, -2, -1, -3, 
                                          -2, 5, -1, -3, -1, 0, -1, -3, -2, -2}, 
                                        {-1, -1, -2, -3, -1, 0, -2, -3, -2, 1, 
                                          2, -1, 5, 0, -2, -1, -1, -1, -1, 1}, 
                                        {-2, -3, -3, -3, -2, -3, -3, -3, -1, 0, 
                                          0, -3, 0, 6, -4, -2, -2, 1, 3, -1}, 
                                        {-1, -2, -2, -1, -3, -1, -1, -2, -2, 
                                          -3, -3, -1, -2, -4, 7, -1, -1, -4, -3, -2}, 
                                        {1, -1, 1, 0, -1, 0, 0, 0, -1, -2, -2, 
                                          0, -1, -2, -1, 4, 1, -3, -2, -2}, 
                                        {0, -1, 0, -1, -1, -1, -1, -2, -2, -1, 
                                          -1, -1, -1, -2, -1, 1, 5, -2, -2, 0}, 
                                        {-3, -3, -4, -4, -2, -2, -3, -2, -2, 
                                          -3, -2, -3, -1, 1, -4, -3, -2, 11, 2, -3}, 
                                        {-2, -2, -2, -3, -2, -1, -2, -3, 2, -1, 
                                          -1, -2, -1, 3, -3, -2, -2, 2, 7, -1}, 
                                        {0, -3, -3, -3, -1, -2, -2, -3, -3, 3, 
                                          1, -2, 1, -1, -2, -2, 0, -3, -1, 4}};
	//DISORDER
	//static const std::vector< std::vector<int> > simScores = {{3, -2, -1, -1, -1, -1, -1, 0, -2, -1, -1, -1, -1, -2, -1, 1, 0, -5, -2, 0}, {-2, 5, 0, -2, -1, 1, -1, -2, 0, -2, -2, 2, -1, -3, -2, -1, -1, 0, -2, -2}, {-1, 0, 4, 1, -1, 1, 0, 0, 2, -3, -3, 0, -2, -2, -1, 1, 0, -3, -1, -3}, {-1, -2, 1, 4, -3, 0, 2, -1, -1, -4, -4, -1, -4, -4, -2, 0, -1, -4, -4, -4}, {-1, -1, -1, -3, 10, -3, -4, -3, -1, 0, -1, -3, 0, -1, -2, 0, 1, -5, 0, 1}, {-1, 1, 1, 0, -3, 5, 0, -2, 1, -2, -2, 0, -1, -2, -1, 0, 0, -1, 0, -2}, {-1, -1, 0, 2, -4, 0, 4, -2, -1, -3, -3, 0, -3, -4, -1, -1, -1, -4, -3, -2}, {0, -2, 0, -1, -3, -2, -2, 5, -1, -5, -4, -2, -4, -4, -1, 0, -2, -4, -3, -4}, {-2, 0, 2, -1, -1, 1, -1, -1, 8, -2, -2, -1, -2, 0, -2, -1, 0, -2, 2, -2}, {-1, -2, -3, -4, 0, -2, -3, -5, -2, 4, 2, -2, 1, 1, -2, -2, -1, -2, 0, 3}, {-1, -2, -3, -4, -1, -2, -3, -4, -2, 2, 4, -2, 2, 1, -1, -2, -2, -2, 0, 1}, {-1, 2, 0, -1, -3, 0, 0, -2, -1, -2, -2, 4, -2, -3, -1, -1, 0, -3, -2, -2}, {-1, -1, -2, -4, 0, -1, -3, -4, -2, 1, 2, -2, 7, 1, -2, -2, -1, -1, -1, 1}, {-2, -3, -2, -4, -1, -2, -4, -4, 0, 1, 1, -3, 1, 7, -3, -2, -2, -1, 4, 0}, {-1, -2, -1, -2, -2, -1, -1, -1, -2, -2, -1, -1, -2, -3, 6, 0, -1, -1, -3, -1}, {1, -1, 1, 0, 0, 0, -1, 0, -1, -2, -2, -1, -2, -2, 0, 3, 1, -3, -2, -2}, {0, -1, 0, -1, 1, 0, -1, -2, 0, -1, -2, 0, -1, -2, -1, 1, 4, -5, -1, 0}, {-5, 0, -3, -4, -5, -1, -4, -4, -2, -2, -2, -3, -1, -1, -1, -3, -5, 13, 3, -4}, {-2, -2, -1, -4, 0, 0, -3, -3, 2, 0, 0, -2, -1, 4, -3, -2, -1, 3, 8, -1}, {0, -2, -3, -4, 1, -2, -2, -4, -2, 3, 1, -2, 1, 0, -1, -2, 0, -4, -1, 4}};
	//dummy 0 5
	//static const std::vector< std::vector<int> > simScores = {{5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5}};
	//DUNKER extended alphabet	
	//static const std::vector< std::vector<int> > simScores = {{9, 1, 2, 2, 1, 3, 2, 2, 1, 1, 1, 2, 0, 0, 3, 3, 4, -1, 0, 3}, {1, 10, 1, 0, 1, 3, 1, 2, 3, 0, 0, 5, -1, -2, 0, 1, 1, 2, -1, 0}, {2, 1, 11, 4, 1, 3, 2, 2, 3, 0, -1, 3, -1, -1, 1, 4, 3, -2, 1, 0}, {2, 0, 4, 10, -2, 2, 5, 2, 2, -1, -2, 1, -2, -2, 1, 2, 2, -3, -1, 0}, {1, 1, 1, -2, 17, 0, -3, 1, 0, 0, 0, -1, -2, 1, -1, 2, 1, 3, 2, 1}, {3, 3, 3, 2, 0, 11, 4, 0, 4, 0, 1, 3, 0, -1, 2, 2, 2, 1, 0, 1}, {2, 1, 2, 5, -3, 4, 9, 1, 1, -1, -1, 3, -1, -2, 0, 1, 1, -2, -2, 0}, {2, 2, 2, 2, 1, 0, 1, 10, 0, -2, -2, 0, -2, -2, 0, 2, 1, 0, -2, 0}, {1, 3, 3, 2, 0, 4, 1, 0, 13, 0, 0, 1, -1, 2, 1, 1, 1, 1, 4, 0}, {1, 0, 0, -1, 0, 0, -1, -2, 0, 12, 5, 0, 4, 4, 0, 0, 2, 1, 2, 7}, {1, 0, -1, -2, 0, 1, -1, -2, 0, 5, 10, -1, 4, 5, 1, 0, 1, 2, 2, 4}, {2, 5, 3, 1, -1, 3, 3, 0, 1, 0, -1, 10, -1, -2, 0, 1, 2, -2, -1, 0}, {0, -1, -1, -2, -2, 0, -1, -2, -1, 4, 4, -1, 13, 2, -2, -1, 1, 0, 0, 3}, {0, -2, -1, -2, 1, -1, -2, -2, 2, 4, 5, -2, 2, 13, -1, 0, 0, 6, 8, 3}, {3, 0, 1, 1, -1, 2, 0, 0, 1, 0, 1, 0, -2, -1, 11, 2, 2, -1, -2, 1}, {3, 1, 4, 2, 2, 2, 1, 2, 1, 0, 0, 1, -1, 0, 2, 9, 4, -1, 0, 1}, {4, 1, 3, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 0, 2, 4, 10, -2, 0, 3}, {-1, 2, -2, -3, 3, 1, -2, 0, 1, 1, 2, -2, 0, 6, -1, -1, -2, 18, 6, 0}, {0, -1, 1, -1, 2, 0, -2, -2, 4, 2, 2, -1, 0, 8, -2, 0, 0, 6, 14, 1}, {3, 0, 0, 0, 1, 1, 0, 0, 0, 7, 4, 0, 3, 3, 1, 1, 3, 0, 1, 11}};
	//dummy binary
	//static const std::vector<std::vector<int> > simScores = {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};
}


int substitutionMatrix::getElement(char char1, char char2){
	int index1=0;
	int index2=0;
  for (unsigned int i = 0; i < alphabet.size(); i++){
		if(alphabet[i] == char1){
			index1 = i;
		}
		if(alphabet[i] == char2){
			index2 = i;
		}
	}
	return simScores[index1][index2];
}


profile_matrix substitutionMatrix::convertToProfileFormat(sequence& seq){
	profile_matrix result(seq.size());
	sbstMatColumnsList newSbstRow;
	for (unsigned int i = 0; i < result.size(); i++){
		if (seq[i].getAA() == 'B'){
			newSbstRow.clear();
			newSbstRow.push_back(simScores[2]);
			newSbstRow.push_back(simScores[3]);
			result[i] = vecUtil::average(newSbstRow);
		}
		else if (seq[i].getAA() == 'Z'){
			newSbstRow.clear();
			newSbstRow.push_back(simScores[6]);
			newSbstRow.push_back(simScores[7]);
			result[i] = vecUtil::average(newSbstRow);

		}
		else if (seq[i].getAA() == 'X'){
			result[i] = vecUtil::average(simScores);
		}
		else{
			int aAcidInt = findAminoAcidsNo(seq[i].getAA());
      sbstMatColumn sbst_column_int = simScores[aAcidInt];
			result[i] = vecUtil::convertIntVectorToDoubleVector(sbst_column_int);//adds a column to the result(converted from int to double)
		}
	}
	vecUtil::transposeVec(result);
	return result;
}


int substitutionMatrix::getElement(int i,int j){
	return simScores[i][j];
}


void substitutionMatrix::getColumn(unsigned int& columnNo, 
                                   sbstMatColumn& column_int){
	column_int = simScores[columnNo];
}


int substitutionMatrix::findAminoAcidsNo(char aa){
	int aAcidint = -1;
	for (unsigned int i = 0; i < alphabet.size();i++){
		if (aa == alphabet[i]){
			aAcidint = i;
			break;
		}
	}
	return aAcidint;
}
