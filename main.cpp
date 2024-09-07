#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <curl/curl.h>

using namespace std;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *s)
{
     s->append((char *)contents, size * nmemb);
     return size * nmemb;
}

// Function to fetch JSON data from a URL
string fetch_block_data(int blockID)
{
     string url = "https://raw.githubusercontent.com/rajucse/btc_blocks_transaction/main/" + to_string(blockID) + ".json";
     CURL *curl;
     CURLcode res;
     string readBuffer;

     curl = curl_easy_init();
     if (curl)
     {
          curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
          curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
          res = curl_easy_perform(curl);
          curl_easy_cleanup(curl);
     }

     return readBuffer;
}

unordered_map<string, vector<string>> parse_json(const string &jsonData)
{
     unordered_map<string, vector<string>> txGraph;
     size_t pos = 0;
     string token;
     string txID;

     while ((pos = jsonData.find("txid")) != string::npos)
     {
          size_t start = jsonData.find(":", pos) + 2;
          size_t end = jsonData.find("\"", start);
          txID = jsonData.substr(start, end - start);

          pos = jsonData.find("vin", end);
          size_t vin_start = jsonData.find("[", pos);
          size_t vin_end = jsonData.find("]", vin_start);

          while (vin_start < vin_end)
          {
               vin_start = jsonData.find("txid", vin_start);
               if (vin_start == string::npos || vin_start >= vin_end)
                    break;
               start = jsonData.find(":", vin_start) + 2;
               end = jsonData.find("\"", start);
               string parentTxID = jsonData.substr(start, end - start);
               txGraph[txID].push_back(parentTxID);
               vin_start = end;
          }

          pos = vin_end;
     }

     return txGraph;
}

int max_ancestry_depth(const unordered_map<string, vector<string>> &txGraph)
{
     unordered_set<string> allTransactions;
     for (const auto &entry : txGraph)
     {
          allTransactions.insert(entry.first);
     }

     auto calculate_ancestry = [&](const string &txID)
     {
          unordered_set<string> visited;
          queue<string> q;
          q.push(txID);
          int depth = 0;

          while (!q.empty())
          {
               string current = q.front();
               q.pop();
               if (visited.find(current) != visited.end())
                    continue;
               visited.insert(current);

               for (const string &parent : txGraph.at(current))
               {
                    if (allTransactions.find(parent) != allTransactions.end())
                    {
                         q.push(parent);
                         depth++;
                    }
               }
          }

          return depth;
     };

     int maxDepth = 0;
     int varOcg = 0;

     for (const auto &tx : allTransactions)
     {
          int depth = calculate_ancestry(tx);
          if (depth > maxDepth)
          {
               maxDepth = depth;
               varOcg = depth;
          }
     }

     return maxDepth;
}

int AncestryDepthCalculationinBlockchain(int blockID)
{
     // Fetch block data
     string blockData = fetch_block_data(blockID);

     // Parse the block data manually
     unordered_map<string, vector<string>> txGraph = parse_json(blockData);

     // Calculate maximum ancestry depth
     int result = max_ancestry_depth(txGraph);

     return result;
}

int main(void)
{
     int blockID;
     cout << "Enter block ID: ";
     cin >> blockID;

     // Keep this function call here
     cout << AncestryDepthCalculationinBlockchain(blockID) << endl;
     return 0;
}
