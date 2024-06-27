#include <fstream>
#include <sstream>
#include <iostream>
#include "Leaderboard.h"

void insert_at(LeaderboardEntry* &entry, LeaderboardEntry* new_entry) {

    if (entry == nullptr) entry = new_entry;
    else if (new_entry->score > entry->score){

        new_entry->next_leaderboard_entry = entry;
        entry = new_entry;
    }
    else if (entry->next_leaderboard_entry == nullptr) entry->next_leaderboard_entry = new_entry;
    else insert_at(entry->next_leaderboard_entry, new_entry);


}

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {

    insert_at(head_leaderboard_entry, new_entry);

    LeaderboardEntry* current_entry = head_leaderboard_entry;
    for (int i = 0; i < 9; ++i) {

        if (current_entry->next_leaderboard_entry != nullptr) current_entry = current_entry->next_leaderboard_entry;
        else {
            current_entry = nullptr;
            break;
        }
    }
    if (current_entry != nullptr && current_entry->next_leaderboard_entry != nullptr) {
        LeaderboardEntry *delete_after = current_entry->next_leaderboard_entry;
        current_entry->next_leaderboard_entry = nullptr;
        delete delete_after;
    }


}

void Leaderboard::write_to_file(const string& filename) {
    std::ofstream file(filename);

    LeaderboardEntry* current_entry = nullptr;

    if (head_leaderboard_entry != nullptr) {

        do {

            if (current_entry == nullptr) current_entry = head_leaderboard_entry;
            else current_entry = current_entry->next_leaderboard_entry;

            file<<current_entry->score<<" "<<current_entry->last_played<<" "<<current_entry->player_name<<std::endl;

        } while (current_entry->next_leaderboard_entry != nullptr);
    }

    file.close();
}

void Leaderboard::read_from_file(const string& filename) {

    std::ifstream file(filename);
    std::string line;

    unsigned long score;
    time_t last_played;
    std::string name;

    LeaderboardEntry* current_entry = nullptr;

    while (getline(file, line)) {

        stringstream split_line(line);
        split_line >> score;
        split_line >> last_played;
        split_line >> name;

        if (current_entry == nullptr) current_entry = head_leaderboard_entry =
                    new LeaderboardEntry{score, last_played, name};
        else {

            current_entry->next_leaderboard_entry = new LeaderboardEntry{score, last_played, name};
            current_entry = current_entry->next_leaderboard_entry;
        }
    }
}


void Leaderboard::print_leaderboard() {

    int i(0);
    LeaderboardEntry* current_entry = nullptr;

    std::cout<<"Leaderboard\n-----------\n";
    if (head_leaderboard_entry != nullptr){
        do {

            if(current_entry == nullptr) current_entry = head_leaderboard_entry;
            else current_entry = current_entry->next_leaderboard_entry;

            struct tm formatter = *localtime(&current_entry->last_played);

            std::cout<<++i<<". "<<current_entry->player_name<<" "<<current_entry->score<<" ";
            std::cout<<formatter.tm_hour<<":"<<formatter.tm_min<<":"<<formatter.tm_sec<<"/";
            std::cout<<formatter.tm_mday<<"."<<formatter.tm_mon+1<<"."<<formatter.tm_year+1900<<std::endl;

        } while (current_entry->next_leaderboard_entry != nullptr);
    }

}

Leaderboard::~Leaderboard() {
    delete head_leaderboard_entry;
}
