#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include <vector>
#include "factorization.h"

struct triple {
unsigned long a;
unsigned long b;
unsigned long c;
unsigned long sum;
} ;

std::vector< triple> * check(unsigned long fluffy_number);

int count = 0, num_divisors = 0;
unsigned long gamma_bound = 0;
bool pure_only;

int main(int argc, char* argv[]) {
  std::string file_string = "output.txt";

  unsigned long lower_limit, upper_limit;
  bool show_progress_bar = false;
  std::string searching_for;
  std::cout << "The Fluffy Forager, v5" << std::endl << std::endl; // v1 - v4 written in Java
  std::cout << "Welcome! Please set the options for your search." << std::endl;
  std::cout << "Press enter to use the default value (in parantheses)." << std::endl << std::endl;

  while (true) {
    std::cout << "Enter lower limit (0): ";
    std::string lower_limit_temp;
    std::getline(std::cin, lower_limit_temp);
    if (lower_limit_temp.empty()) {
      lower_limit = 0;
      break;
    }
    else try {
      lower_limit = stol(lower_limit_temp);
    }
    catch (std::invalid_argument) {
      std::cout << ">> Invalid input - try again." << std::endl;
      continue;
    }
    if (lower_limit > 4294967295) { // (2 * 2^31 - 1)
      std::cout << ">> Number too high - try again." << std::endl;
    }
    else {
      break;
    }
  }

  while (true) {
    std::cout << "Enter upper limit (1000000): ";
    std::string upper_limit_temp;
    std::getline(std::cin, upper_limit_temp);
    if (upper_limit_temp.empty() && lower_limit < 1000000) {
      upper_limit = 1000000;
      break;
    }
    else try {
      upper_limit = stol(upper_limit_temp);
    }
    catch (std::invalid_argument) {
      std::cout << ">> Invalid input - try again." << std::endl;
      continue;
    }
    if (upper_limit < lower_limit) {
      std::cout << ">> Number too low - try again." << std::endl;
      continue;
    }
    else if (upper_limit > 4294967295) { // (2 * 2^31 - 1)
      std::cout << ">> Number too high - try again." << std::endl;
    }
    else {
      break;
    }
  }

  while (true) {
    std::cout << "Enter gamma bound - 0 to disable (0): ";
    std::string gamma_bound_temp;
    std::getline(std::cin, gamma_bound_temp);
    if (gamma_bound_temp.empty() || gamma_bound_temp == "0") {
      gamma_bound = 0;
      break;
    }
    else try {
      gamma_bound = stol(gamma_bound_temp);
    }
    catch (std::invalid_argument) {
      std::cout << ">> Invalid input - try again." << std::endl;
      continue;
    }
    unsigned long highest_possible = (unsigned long)(pow(gamma_bound, 3));
    if (gamma_bound > 1625) { // (2* 2^31 - 1)^(1/3)
      std::cout << ">> Number too high - try again." << std::endl;
    }
    else {
      if (lower_limit > highest_possible) {
        std::cout << ">> Bound invalid - all possible outputs are outside limits." << std::endl;
        continue;
      }
      if (upper_limit > highest_possible) {
        upper_limit = highest_possible;
        std::cout << "Setting upper limit to highest possible value (" << upper_limit << ")." << std::endl;
        break;
      }
      break;
    }
  }

  while (true) {
    std::cout << "Search for pure fluffy numbers only? y/n (n): ";
    std::string pure_only_temp;
    std::getline(std::cin, pure_only_temp);
    if (pure_only_temp.empty() || pure_only_temp == "n") {
      pure_only = false;
      break;
    }
    else if (pure_only_temp == "y") {
      pure_only = true;
      break;
    }
    else {
      std::cout << "Invalid input - try again." << std::endl;
      continue;
    }
  }

  if (gamma_bound != 0) {
    searching_for = "fluffy numbers (gamma{" + std::to_string(gamma_bound) + "})";
  }
  else{
    searching_for = "fluffy numbers (all)";
  }

  if (pure_only && gamma_bound != 0) {
    searching_for = "fluffy numbers (pure, gamma{" + std::to_string(gamma_bound) + "})";
  }
  else if (pure_only && gamma_bound == 0) {
    searching_for = "fluffy numbers (pure)";
  }
  else if (!pure_only && gamma_bound != 0) {
    searching_for = "fluffy numbers (gamma{" + std::to_string(gamma_bound) + "})";
  }
  else{
    searching_for = "fluffy numbers (all)";
  }

  unsigned long interval = (unsigned long)((upper_limit - lower_limit) / 50.0);

  std::cout << std::endl << "Configuration complete. Press Enter to begin searching...";
  std::cin.get();

  std::ofstream file;
  file.open(file_string);
  file << "The Fluffy Forager, v5" << std::flush;

  std::cout << std::endl << "Search criteria: " << searching_for << "; "
      << "limits: " << lower_limit << " <= x <= " << upper_limit << std::endl;
  file << std::endl << "Search criteria: " << searching_for << "; "
      << "limits: " << lower_limit << " <= x <= " << upper_limit << std::endl << std::flush;
  if (interval > 30) {
    show_progress_bar = true;
  }
  if (show_progress_bar) {
    std::string pbar = " ** PLEASE WAIT - SEARCH IN PROGRESS **";
    for (int i = pbar.length(); i < 50; i++) {
      pbar += " ";
    }
    std::cout << " 0% |" << pbar << "| 100%" << std::endl;
    std::cout << "    |";
  }
  else {
    std::cout << "**PLEASE WAIT - SEARCH IN PROGRESS**" << std::endl;
  }
  file << std::endl << "      #|  dvs|     a|     b|     c|   sum|" << std::endl << std::endl << std::flush;

  bool mark = false;

  std::vector< triple> * test;

//std::cout << setfill(' ') << setw(7) << 5;

  for (unsigned long i = lower_limit; i <= upper_limit; i++) {
    test = check(i);
    if (test != NULL) {
      mark = true;
      for (int j = 0; j < (*test).size(); j++) {
        if (j == 0) {
          file << std::setfill(' ') << std::setw(7) << i;
          file << std::setfill(' ') << std::setw(6) << num_divisors;
        }
        else {
          file << "             ";
        }
        file << std::setfill(' ') << std::setw(7) << (*test)[j].a << std::setw(7) << (*test)[j].b << std::setw(7) << (*test)[j].c << std::flush; // prints triple elements
        if (j == 0) {
          file << std::setfill(' ') << std::setw(7) << (*test)[j].sum; // prints sum
        }
        file << std::endl;
      }
    }
    delete test;
    if (show_progress_bar && i % interval == interval - 1) {
      if (mark) {
        std::cout << "=";
        mark = false;
      }
      else {
        std::cout << "-";
      }
      std::cout << std::flush;
    }
  }
  if (show_progress_bar) {
    std::cout << "!";
  }
  double elapsed_time = 0;
  std::cout << std::endl << "Search completed in " << std::to_string(elapsed_time) << " seconds. "
      << std::to_string(count) << " " << searching_for << " found." << std::endl
      << "Results have been saved to output.txt. "
      << "If you are using Terminal," << std::endl
      << "you may type \"cat output.txt\" to view the results." << std::endl;
  file << std::endl << "Search completed in " << std::to_string(elapsed_time) << " seconds. "
      << std::to_string(count) << " " << searching_for << " found." << std::endl;

  file.close();
  return 0;
}

std::vector< triple> * check(unsigned long fluffy_number) {
  if (fluffy_number == 0) {
    return NULL;
  }

  std::vector<unsigned long> divisors = factorization::deduce_divisors(fluffy_number);
  num_divisors = divisors.size();

  if (num_divisors < 4) {
    return NULL;
  }

  std::vector<triple> * matches = new std::vector< triple>();
  std::vector<triple> * valid_matches = new std::vector< triple>();

  bool flag = false; // following code could use some more optimization
  for (int i = 0; i < divisors.size(); i++) {
    unsigned long a = divisors[i];
    for (int j = i; j < divisors.size(); j++) {
      unsigned long b = divisors[j];
      for (int k = j; k < divisors.size(); k++) {
        unsigned long c = divisors[k];
        if (a * b * c > fluffy_number) {
          break;
        }
        if ((a * b * c == fluffy_number)) {
           triple new_triple = {a, b, c, a + b + c};
          matches -> push_back(new_triple);
          break;
        }
      }
    }
  }
  flag = false;
  for (int i = 0; i < matches->size(); i++) {
    for (int j = 0; j < matches->size(); j++) {
      if ((((*matches)[i].a != (*matches)[j].a) && (*matches)[i].b != (*matches)[j].b)
          && (((*matches)[i].sum == (*matches)[j].sum)) && (i != j)) {
        valid_matches -> push_back((*matches)[i]);
        flag = true;
      }
    }
  }
  delete matches;
  if (flag) {
    if (valid_matches -> size() == 2) {
      bool keep_sorting = true;
      while (keep_sorting) {
        keep_sorting = false;
        int i = 0;
        while (i < valid_matches -> size() - 1) {
          if ((*valid_matches)[i].sum > (*valid_matches)[i + 1].sum) {
             triple temp = (*valid_matches)[i];
            (*valid_matches)[i] = (*valid_matches)[i + 1];
            (*valid_matches)[i + 1] = temp;
            keep_sorting = true;
            i++;
          }
          else if ((*valid_matches)[i].a == (*valid_matches)[i + 1].a) {
            valid_matches -> erase(valid_matches -> begin() + i - 1);
          }
          else {
            i++;
          }
        }
      }
      if (gamma_bound != 0) {
        if ((*valid_matches)[0].a > gamma_bound
            || (*valid_matches)[0].b > gamma_bound
            || (*valid_matches)[0].c > gamma_bound
            || (*valid_matches)[1].a > gamma_bound
            || (*valid_matches)[1].b > gamma_bound
            || (*valid_matches)[1].c > gamma_bound) {
          delete valid_matches;
          return NULL;
        }
      }
      if (!pure_only && ((*valid_matches)[0].b == (*valid_matches)[0].c
          && (*valid_matches)[1].a != (*valid_matches)[1].b)) {
        count++;
        return valid_matches;
      }
      else if (((*valid_matches)[0].b == (*valid_matches)[0].c
          && (*valid_matches)[1].a == (*valid_matches)[1].b)) {
        count++;
        return valid_matches;
      }
      else {
        delete valid_matches;
        return NULL;
      }
    }
    else {
      delete valid_matches;
      return NULL;
    }
  }
  else {
    delete valid_matches;
    return NULL;
  }

  delete valid_matches;
  return NULL;
}
