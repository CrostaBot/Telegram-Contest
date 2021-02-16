#include "tgcat.h"
#include "../resources/fastText/src/fasttext.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;
using namespace fasttext;

map<string, int> set_label = {
  {"Art", 0},
  {"Bets", 1},
  {"Books", 2},
  {"Business", 3},
  {"Cars", 4},
  {"Celebrities", 5},
  {"Cryptocurrencies", 6},
  {"Culture", 7},
  {"Curious", 8},
  {"Directories", 9},
  {"Economy", 10},
  {"Education", 11},
  {"Erotic", 12},
  {"Fashion", 13},
  {"Fitness", 14},
  {"Food", 15},
  {"Foreign", 16},
  {"Health", 17},
  {"History", 18},
  {"Hobbies", 19},
  {"Home", 20},
  {"Humor", 21},
  {"Investments", 22},
  {"Job", 23},
  {"Kids", 24},
  {"Marketing", 25},
  {"Motivation", 26},
  {"Movies", 27},
  {"Music", 28},
  {"Offers", 29},
  {"Pets", 30},
  {"Politics", 31},
  {"Psychology", 32},
  {"Real", 33},
  {"Recreation", 34},
  {"Religion", 35},
  {"Science", 36},
  {"Sports", 37},
  {"Technology", 38},
  {"Travel", 39},
  {"Video", 40},
  {"Other", 41}
};

FastText Language_Recognition;
FastText Topic_RU;
FastText Topic_EN;

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}

string clear_string(string s)
{
	int len = s.size();
	if (len != 0) {

	  transform(s.begin(), s.end(), s.begin(), ::tolower);

	  replace_if(s.begin(), s.end(),  [](char c) { return!isalpha(c); }, ' ');

	  string::iterator new_end = std::unique(s.begin(), s.end(), BothAreSpaces);
	  s.erase(new_end, s.end());

	  if(s[0] == ' '){
	    s.erase(0, 1);
	  }

	  int leng = s.size();
	  if(s[leng-1] == ' '){
	    s.erase(leng - 1);

	}
}
  return s; 
}

int tgcat_init() { 

  Language_Recognition.loadModel("../resources/Models/lid.176.bin");
  Topic_RU.loadModel("../resources/Models/model_ru.bin");
  Topic_EN.loadModel("../resources/Models/model_en.bin");

  return 0;
}

int tgcat_detect_language(const struct TelegramChannelInfo *channel_info,
                          char language_code[6]) {
  
  const char *t = channel_info -> title;
  const char *d = channel_info -> description;
  const int total_posts = channel_info -> post_count;
  const char **p = channel_info -> posts;
  
  string tit = t;
  string desc = d;
  vector<string> files(p, p + total_posts);

  string title = clear_string(t);
  string description = clear_string(d);

  string messages;
  string message;
  for(int i = 0; i < total_posts; i++) {
    message = clear_string(p[i]);
    messages = messages + message + ' ';
  }

  string::iterator new_end = std::unique(messages.begin(), messages.end(), BothAreSpaces);
  messages.erase(new_end, messages.end());

  istringstream ifs(messages);
  vector<pair<real, string>> predictions;

  Language_Recognition.predictLine(ifs, predictions, 1, 0.0);

  string label;
  if (!predictions.empty()){

    const size_t label_prefix = 9;
    label = predictions[0].second.substr(label_prefix);

  	if (label.size() != 2) {
    	label = "other";
    }

    strcpy(language_code, label.c_str());

  	return 0; 

  } else {
  		return -1;
  }

}

int tgcat_detect_category(const struct TelegramChannelInfo *channel_info,
                          double category_probability[TGCAT_CATEGORY_OTHER + 1]) {
  const char *t = channel_info -> title;
  const char *d = channel_info -> description;
  const int total_posts = channel_info -> post_count;
  const char **p = channel_info -> posts;
  
  string tit = t;
  string desc = d;
  vector<string> files(p, p + total_posts);

  string title = clear_string(t);
  string description = clear_string(d);

  string messages;
  string message;
  for(int i = 0; i < total_posts; i++) {
    message = clear_string(p[i]);
    messages = messages + message + ' ';
  }

  string::iterator new_end = std::unique(messages.begin(), messages.end(), BothAreSpaces);
  messages.erase(new_end, messages.end());
  
  istringstream ifs(messages);
  vector<pair<real, string>> predictions_language;
  vector<pair<real, string>> predictions_topic;

  string text = title + " " + description + " " + messages;
  istringstream topic(text);
  
  Language_Recognition.predictLine(ifs, predictions_language, 1, 0.0);

  if (!predictions_language.empty()){
 	const size_t label_prefix = 9;
  	string label = predictions_language[0].second.substr(label_prefix);

	if (label == "en") {
    	Topic_EN.predictLine(topic, predictions_topic, 41, 0.0);
    	string label_topic;
		
		int i = 0;
		int position = 0;
		double topic_prob;
		map<string, int>::iterator it;
		for (const auto& p : predictions_topic) {
    		label_topic = predictions_topic[i].second.substr(label_prefix);
    		it = set_label.find(label_topic);
    		position = it->second;
   	 		topic_prob = predictions_topic[i].first;
   			category_probability[position] = topic_prob; 
    		i++;
  		}

  		return 0;

  	} else if (label == "ru") {
    	Topic_RU.predictLine(topic, predictions_topic, 41, 0.0);
    	string label_topic;
		
		int i = 0;
		int position = 0;
		double topic_prob;
		map<string, int>::iterator it;
		for (const auto& p : predictions_topic) {
    		label_topic = predictions_topic[i].second.substr(label_prefix);
    		it = set_label.find(label_topic);
    		position = it->second;
   	 		topic_prob = predictions_topic[i].first;
   			category_probability[position] = topic_prob; 
    		i++;
  		}

  		return 0; 

  	} else {
  		return -1;
  }

  }

}
