#pragma once

#include <array>
#include <string>


static const int MAX_SCORES = 5;


struct ScoreEntry
{
    char initials[5] {0}; // 4 letters + null terminator
    int score {0};
    int wave {0};
};


class Scoreboard
{
public:
    static Scoreboard& instance();

    void load();
    void save();
    void addEntry(const char* initials, int score, int wave);
    void reset();

    const std::array<ScoreEntry, MAX_SCORES>& getEntries() const { return entries; }
    int getCount() const { return count; }

private:
    Scoreboard() = default;
    Scoreboard(const Scoreboard&) = delete;

    std::array<ScoreEntry, MAX_SCORES> entries {};
    int count {0};

    const std::string filepath {"scores.txt"};
};