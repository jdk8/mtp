#ifndef WORD_COUNT_H__
#define WORD_COUNT_H__

#include <mtp/server_actor.h>
#include <unordered_map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "word_count.pb.h"

class WordCount : public mtp::ServiceActor {
 public:
  WordCount() {
    set_instance(8);
  }

  void Init() {
    LOG_INFO << "WordCount Init";
    std::ifstream in_file("data/" + std::to_string(shard_no()+1) + ".txt");
    std::string line;
    while(getline(in_file, line)) {
      boost::trim(line);
      auto iter = word_counts_.find(line);
      if (iter != word_counts_.end()) {
         iter->second += 1;
      } else {
        word_counts_[line] = 1;
      }
    }
    WordCountPairs word_count_pairs;
    for(auto& kv : word_counts_) {
      auto word_count_pair = word_count_pairs.add_word_count_pairs();
      word_count_pair->set_word(kv.first);
      word_count_pair->set_count(kv.second);
    }
    if (shard_no() > 0) {
      CallShardService("WordCount", 0, "RecvWordCountPairs", word_count_pairs);
    }
  }

  void RecvWordCountPairs(const WordCountPairs& word_count_pairs) {
    counter_ += 1;
    for (int i = 0; i < word_count_pairs.word_count_pairs_size(); i++) {
      auto word_count_pair = word_count_pairs.word_count_pairs(i);
      const std::string& word = word_count_pair.word();
      int count = word_count_pair.count();
      auto iter = word_counts_.find(word);
      if (iter != word_counts_.end()) {
        iter->second += count;
      } else {
        word_counts_[word] = count;
      }
    }
    if (counter_ == 7) {
      LOG_INFO << "Word count result:";
      for(auto& kv : word_counts_) {
        LOG_INFO << kv.first << "," << kv.second;
      }
    }
  }

 private:
  std::unordered_map<std::string, int> word_counts_;
  int counter_ = 0;
};

#endif
