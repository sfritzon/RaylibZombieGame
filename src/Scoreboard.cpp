#include "Scoreboard.h"
#include <fstream>
#include <cstring>
#include <algorithm>


Scoreboard& Scoreboard::instance()
{
    static Scoreboard scoreboard;
    return scoreboard;
}


void Scoreboard::load()
{
    count = 0;
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    while (count < MAX_SCORES)
    {
        ScoreEntry entry {};
        file >> entry.initials >> entry.score >> entry.wave;
        if (file.fail()) break;
        entries[count++] = entry;
    }
}


void Scoreboard::save()
{
    std::ofstream file(filepath);
    if (!file.is_open()) return;

    for (int i = 0; i < count; i++)
    {
        file << entries[i].initials << " " << entries[i].score << " " << entries[i].wave << "\n";
    }
}


void Scoreboard::addEntry(const char* initials, int score, int wave)
{
    ScoreEntry entry {};
    strncpy(entry.initials, initials, 4);
    entry.initials[4] = '\0';
    entry.score = score;
    entry.wave = wave;

    if (count < MAX_SCORES)
        entries[count++] = entry;
    else if (score > entries[MAX_SCORES - 1].score)
        entries[MAX_SCORES - 1] = entry;
    else
        return;

    std::sort(entries.begin(), entries.begin() + count, [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });

    save();
}


void Scoreboard::reset()
{
    entries = {};
    count = 0;
    std::ofstream file(filepath, std::ofstream::trunc);
}