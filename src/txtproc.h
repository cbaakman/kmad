#ifndef TXTPROC_H
#define TXTPROC_H

#include "sequences.h"

#include <iostream>
#include <string>
#include <vector>


class FeaturesProfile;
class Sequences;
namespace txtproc{
  ///
  /// writes a line of text from the istream to string t 
  /// (removes newline characters)
  ///
  std::istream& safeGetline(std::istream& is, std::string& t);
  ///
  /// splits a string by the delimiter, returns a vector of strings
  ///
  std::vector<std::string> split(const std::string& s, char delim);
  ///
  /// checks if character from the fasta file is on the list of accepetd
  /// characters
  ///
  bool acceptedChar(char my_char);
  ///
  /// writes alignment to file (encoded)
  ///
  void writeAlignmentToFile(StringSequences& sequences, 
                            SeqNames& sequence_names,
                            std::string filename);
  ///
  /// writes alignment to a file in the regular fasta format (decoded)
  ///
  void writeAlignmentWithoutCodeToFile(StringSequences& sequences, 
                                       SeqNames& sequence_names, 
                                       std::string filename, int codon_length);
  ///
  /// reades configuration file, adds user defined features and their alignment
  /// rules
  ///
  void process_conf_file(std::string filename, FeaturesProfile& feat_profile, 
                         Sequences& sequences_aa);
  ///
  /// from a conf_file string creates a list of indexes of features to be
  /// scored
  ///
  FeaturesList unfold(std::string conf_string, 
                      FeatureNamesList& list_of_features);
}

#endif /* TXTPROC_H */
