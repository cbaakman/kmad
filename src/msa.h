#ifndef MSA_H
#define MSA_H

#include "seq_data.h"
#include "feature_scores.h"
#include "f_config.h"
#include "profile.h"


class Sequences;


namespace msa {
  /// performs the full multiple sequence alignment, returns aligned sequences
  std::vector<fasta::SequenceList> run_msa(
      const seq_data::SequenceData& sequence_data,
      const f_config::FeatureSettingsMap& f_set,
      double gap_open_pen,
      double gap_ext_pen, double end_pen,
      int domain_modifier, int motif_modifier,
      int phosph_modifier, int codon_length, bool one_round,
      const std::string& sbst_mat, bool first_gapped);


  /// performs the first round of alignments, / all vs query seq (first
  /// calculates profile / based only on the query seq, then / aligns all
  /// sequences and calculates / identity of each sequence to the query seq.)
  
  
  std::vector<double> set_identities(
      const seq_data::SequenceData& sequence_data,
      const profile::ProfileMap& profile, FeatureScores& f_profile,
      double penalty, double endPenalty, double extensionPenalty,
      int codon_length);

  ///
  /// performs one round of MSA 
  ///
  
  
  std::vector<fasta::SequenceList> perform_msa_round(
      const seq_data::SequenceData& sequence_data,
      const profile::ProfileMap& profile,
      const FeatureScores& f_profile,
      double gap_open_pen, double end_pen,
      double gap_ext_pen, 
      double identity_cutoff,
      int codon_length, 
      const std::vector<double>& identities,
      int& prev_alignments,
      const f_config::FeatureSettingsMap& f_set,
      std::vector<fasta::SequenceList> previous_alignment, bool gapped);

  ///
  /// takes pairwise alignment, removes
  /// characters from the 2nd sequence that match gaps from 1st seq and returns
  /// vector<string> of 2 elements, where the 1st one is 2nd sequence with cut
  /// out chars and 2nd one
  /// is 2nd sequence with cut out chars and lowercase chars
  /// before and after that
  ///
  fasta::SequenceList remove_gaps(const fasta::SequenceList& alignment);
  ///
  /// takes a sequence and profiles, returns an
  /// alignment of the two, with gaps cut out
  ///
  fasta::SequenceList align_pairwise(const fasta::Sequence& input_sequence, 
                                     const profile::ProfileMap& profile, 
                                     const FeatureScores& f_profile,
                                     double gap_open_pen, double end_pen,
                                     double gap_ext_pen, int codon_length,
                                     bool gapped);

  ///
  /// calculates identity with the query sequence
  /// @param alignedSequence sequence aligned to the profile with the gaps cut
  /// out (its length is equal to the profile's length)
  ///
  double calc_identity(const fasta::Sequence& aligned_sequence, 
                       const fasta::Sequence& query_sequence);

  ///
  /// count alignments that will be performed in this round
  ///
  int count_alignments(double identity_cutoff,
                       const std::vector<double>& identities);

  ///
  /// merges multiple pairwise alignments int one multiple sequence alignment
  /// output is: [[seq0, seq0], [seq1, seq1]] -> each element of the vector
  /// consists of two copies of the same aligned sequence (to make it
  /// analogous to output of non-gapped(no gaps in 1st seq) alignment)
  ///
  std::vector<fasta::SequenceList> merge_alignments(
      const std::vector<fasta::SequenceList>& pairwise_alignments);
  ///
  /// adds new sequence from pairwise alignment to a multiple alignment
  /// 
  std::vector<fasta::SequenceList> add_alignment(
      const std::vector<fasta::SequenceList>& multiple_alignment,
      const fasta::SequenceList& pairwise_alignment);
}

#endif /* MSA_H */
