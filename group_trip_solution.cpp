#include <bits/stdc++.h>
using namespace std;
struct User {
    string name;
    int budget;
    int energy;
    set<string> tags;
    bool active = true;
};
struct Activity {
    int id;
    string name;
    int cost;
    int duration;
    int energy;
    string tag;
};
struct Input {
    int N, D, H;
    vector<User> users;
    map<int, Activity> activities;
    vector<string> events;
};
static Input readInput() {
    Input in;
    cin >> in.N >> in.D >> in.H;
    in.users.resize(in.N);
    for (int i = 0; i < in.N; i++) {
        int k;
        cin >> in.users[i].name >> in.users[i].budget >> in.users[i].energy >> k;
        for (int j = 0; j < k; j++) {
            string t; cin >> t;
            in.users[i].tags.insert(t);
        }
        in.users[i].active = true;
    }
    int A; cin >> A;
    for (int i = 0; i < A; i++) {
        Activity a;
        cin >> a.id >> a.name >> a.cost >> a.duration >> a.energy >> a.tag;
        in.activities[a.id] = a;
    }
    int E; cin >> E;
    cin.ignore();
    for (int i = 0; i < E; i++) {
        string line;
        getline(cin, line);
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        in.events.push_back(line);
    }
    return in;
}
static string formatDay(int day, vector<int> ids, int cost, int sat) {
    if (ids.empty()) {
        return "Day " + to_string(day) + ": REST | cost=0 satisfaction=0";
    }
    sort(ids.begin(), ids.end());
    string s = "Day " + to_string(day) + ": ";
    for (size_t i = 0; i < ids.size(); i++) {
        if (i) s += ' ';
        s += to_string(ids[i]);
    }
    s += " | cost=" + to_string(cost) + " satisfaction=" + to_string(sat);
    return s;
}

int calcSat(const vector<int>& ids, const map<int,Activity>& acts, const vector<User>& users) {
    int sat = 0;
    for (int id : ids) {
        const string& tag = acts.at(id).tag;
        for (auto& u : users)
            if (u.active && u.tags.count(tag)) sat++;
    }
    return sat;
}

tuple<vector<int>,int,int> planDay(
    const vector<int>& eligible,
    const map<int,Activity>& acts,
    const vector<User>& users,
    int H)
{
    int minBudget = INT_MAX, minEnergy = INT_MAX;
    for (auto& u : users) {
        if (!u.active) continue;
        minBudget = min(minBudget, u.budget);
        minEnergy = min(minEnergy, u.energy);
    }
    if (minBudget == INT_MAX) { minBudget = 0; minEnergy = 0; }

    int n = eligible.size();
    vector<int> bestIds;
    int bestSat = -1, bestCost = 0;

    for (int mask = 1; mask < (1 << n); mask++) {
        int totalCost = 0, totalEnergy = 0, totalDur = 0;
        vector<int> ids;
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue;
            int id = eligible[i];
            auto& a = acts.at(id);
            totalCost += a.cost;
            totalEnergy += a.energy;
            totalDur += a.duration;
            ids.push_back(id);
        }
        if (totalCost > minBudget) continue;
        if (totalEnergy > minEnergy) continue;
        if (totalDur > H) continue;

        int sat = calcSat(ids, acts, users);
        sort(ids.begin(), ids.end());
        bool better = false;
        if (sat > bestSat) better = true;
        else if (sat == bestSat) {
            if (totalCost < bestCost) better = true;
            else if (totalCost == bestCost && (bestIds.empty() || ids < bestIds)) better = true;
        }
        if (better) {
            bestSat = sat;
            bestCost = totalCost;
            bestIds = ids;
        }
    }
    return {bestIds, bestCost, max(0, bestSat)};
}

struct EventParsed {
    string type;
    int day;
    string arg1;
    int val = 0;
    string raw;
};

EventParsed parseEvent(const string& line) {
    EventParsed e; e.raw = line;
    istringstream ss(line);
    ss >> e.type;
    if (e.type == "WEATHER") { ss >> e.day >> e.arg1; }
    else if (e.type == "DROP") { ss >> e.day >> e.arg1; }
    else if (e.type == "FATIGUE") { ss >> e.day >> e.arg1 >> e.val; }
    else if (e.type == "BUDGET") { ss >> e.day >> e.arg1 >> e.val; }
    return e;
}

void applyEvent(const EventParsed& ev, vector<User>& users, map<int,set<string>>& wBlocks) {
    if (ev.type == "WEATHER") wBlocks[ev.day].insert(ev.arg1);
    else if (ev.type == "DROP") for (auto& u:users) if(u.name==ev.arg1) u.active=false;
    else if (ev.type == "FATIGUE") for (auto& u:users) if(u.name==ev.arg1) u.energy=ev.val;
    else if (ev.type == "BUDGET") for (auto& u:users) if(u.name==ev.arg1) u.budget=ev.val;
}

// Simulate a full trip plan from day `startDay` to D.
// users: state at start of startDay (events 0..firstEventIdx-1 already applied for days < startDay)
// usedActs: activities consumed before startDay
// wBlocks: weather blocks already established
// evs: ALL parsed events
// firstEventIdx: which events to apply during this simulation
// Returns formatted day lines for days [startDay..D]
vector<string> simulate(
    int startDay, int D, int H,
    vector<User> users,
    set<int> usedActs,
    map<int,set<string>> wBlocks,
    const map<int,Activity>& acts,
    const vector<EventParsed>& evs,
    int firstEventIdx)
{
    vector<string> result;
    for (int day = startDay; day <= D; day++) {
        // Apply events that trigger on this day, starting from firstEventIdx
        for (int i = firstEventIdx; i < (int)evs.size(); i++) {
            if (evs[i].day == day) {
                applyEvent(evs[i], users, wBlocks);
            }
        }
        set<string> blocked = wBlocks.count(day) ? wBlocks.at(day) : set<string>{};
        vector<int> eligible;
        for (auto& [id,act]:acts)
            if (!usedActs.count(id) && !blocked.count(act.tag)) eligible.push_back(id);
        auto [chosenIds,cost,sat] = planDay(eligible, acts, users, H);
        result.push_back(formatDay(day, chosenIds, cost, sat));
        for (int id:chosenIds) usedActs.insert(id);
    }
    return result;
}

static string solve(Input in) {
    string out;

    vector<EventParsed> evs;
    for (auto& line : in.events) evs.push_back(parseEvent(line));

    // === Initial plan: NO events applied ===
    auto initLines = simulate(1, in.D, in.H,
        in.users, {}, {}, in.activities, evs, (int)evs.size() /* don't apply any events */);

    out += "=== PLAN ===\n";
    for (auto& l : initLines) out += l + "\n";

    // === Process events ===
    for (int ei = 0; ei < (int)evs.size(); ei++) {
        auto& ev = evs[ei];
        out += "=== EVENT " + to_string(ei+1) + ": " + ev.raw + " ===\n";
        int eventDay = ev.day;

        // Replay days 1..eventDay-1 with events 0..ei-1 applied
        // to get user state and usedActs just before eventDay
        vector<User> users = in.users;
        set<int> usedActs;
        map<int,set<string>> wBlocks;

        for (int day = 1; day < eventDay; day++) {
            // Apply events 0..ei-1 that trigger on this day
            for (int j = 0; j < ei; j++) {
                if (evs[j].day == day) applyEvent(evs[j], users, wBlocks);
            }
            set<string> blocked = wBlocks.count(day)?wBlocks.at(day):set<string>{};
            vector<int> eligible;
            for (auto& [id,act]:in.activities)
                if (!usedActs.count(id) && !blocked.count(act.tag)) eligible.push_back(id);
            auto [chosenIds,cost,sat] = planDay(eligible, in.activities, users, in.H);
            for (int id:chosenIds) usedActs.insert(id);
        }

        // Apply event ei (takes effect from eventDay)
        applyEvent(ev, users, wBlocks);

        // Re-plan from eventDay with events ei+1.. applied going forward
        // But we also need events 0..ei-1 that trigger on days >= eventDay
        // Apply all events from 0..ei-1 that are on days >= eventDay now too
        for (int j = 0; j < ei; j++) {
            if (evs[j].day >= eventDay) applyEvent(evs[j], users, wBlocks);
        }

        // Now simulate from eventDay with remaining events ei+1..
        auto replanLines = simulate(eventDay, in.D, in.H,
            users, usedActs, wBlocks, in.activities, evs, ei+1);
        for (auto& l : replanLines) out += l + "\n";
    }

    return out;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    Input in = readInput();
    cout << solve(in);
    return 0;
}
