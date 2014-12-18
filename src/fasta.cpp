#include "fasta.h"
#include "txtproc.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


namespace fs = boost::filesystem;


fasta::FastaData fasta::parse_fasta(std::string filename, int codon_length) {
  fs::path p(filename);
  if (!fs::exists(p)) {
    throw std::invalid_argument("File not found: " + filename);
  }

  std::ifstream fastafile(filename.c_str());
  std::string line;
  bool in_sequence_section = true;
  fasta::FastaData fd;
  while (std::getline(fastafile, line)) {
    if (line == std::string("## PROBABILITIES")) {
      in_sequence_section = false;
      continue;
    }

    if (in_sequence_section) {
      assert(line.substr(0, 1) == ">");

      std::string description = line.substr(1, line.length());
      std::getline(fastafile, line);
      fd.sequences.push_back(fasta::make_sequence(description, line,
                                                  codon_length));
    } else {
      std::vector<std::string> result;
      boost::split(result, line, boost::is_any_of("\t "));

      if (result.size() != 2) {
        throw std::runtime_error("Invalid probability format: " + line);
      }

      fd.probabilities[result[0]] = std::stod(result[1]);
    }
  }
  fastafile.close();

  return fd;
}


fasta::Sequence fasta::make_sequence(const std::string& description,
                                     const std::string& codons,
                                     int codon_length)
{
  fasta::Sequence s;
  for (unsigned i = 0; i < codons.size(); i += codon_length) {
    // Use boost regular expression because compiler support for c++11
    // regular expressions is incomplete.
    boost::regex re("\\w{" + std::to_string(codon_length) + "}");
    std::string codon = codons.substr(i, codon_length);
    if (!boost::regex_match(codon, re)) {
      throw std::runtime_error("Invalid codon: " + codon);
    }
    fasta::Residue r(codon);
    s.residues.push_back(r);
  }
  return s;
}
