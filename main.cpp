#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "DataSetRegister.h"
#include "Participants.h"

using namespace std;

#define RUN(x) Run(#x, x, ds)

static vector<string> logs;
static bool all_ok = true;

void Run(string method_name, Participant p, const DataSet& ds)
{
    auto start = chrono::steady_clock::now();
    long long res = p(ds.graph, ds.source, ds.sink);
    auto stop = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(stop - start).count();

    bool correct = res == ds.expected;
    all_ok = all_ok && correct;

    ostringstream oss;
    oss << ds.name << "\t" << ds.graph.n << "\t"
        << ds.graph.edges.size() << "\t" << method_name << "\t"
        << ds.expected << "\t" << res << "\t"
        << (correct ? to_string(time) : "-1");
    logs.push_back(oss.str());
    cout << method_name << " : " << ds.name << " : "
         << (correct ? "OK" : "FAIL") << endl;
}

int main()
{
    auto dataSets = GenerateDataSets();
    while (!dataSets.empty())
    {
        auto& ds = dataSets.top();

        // RUN(participant_1);
        // RUN(participant_2);
        // RUN(participant_3);
        // RUN(participant_4);

        (void)ds;
        dataSets.pop();
    }

    if (logs.empty())
    {
        cout << "No algorithms enabled." << endl;
        return 0;
    }

    ofstream fout("release.txt");
    if (!fout)
    {
        cerr << "Cannot open release.txt" << endl;
        return 1;
    }
    fout << "dataset\tvertices\tedges\talgorithm\texpected\tactual\ttime_us\n";
    for (auto& s : logs)
        fout << s << endl;
    return all_ok ? 0 : 1;
}
