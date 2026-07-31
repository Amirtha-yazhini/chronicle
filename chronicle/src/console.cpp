#include <iostream>
#include <string>
#include <vector>
#include "console.hpp"
#include "input_buffer.hpp"
#include "Engine.hpp"

using namespace std;

void console() {
    string in;
    vector<string> history;
    WorldState worldState;

    while (true) {
        cout << "ironclad>\t";
        if (!getline(cin, in) || in.empty()) continue;

        history.push_back(in);

        if (in == ".help") {
            cout << ".quit       Shut down the world engine\n"
                 << ".help       Show this message\n"
                 << ".version    Show engine version\n"
                 << ".status     Show world status\n";
        }
        else if (in == ".version") {
            cout << "Chronicle v0.1 — Ironhold World Engine\n";
        }
        else if (in == ".quit") {
            break;
        }
        else if (in == ".status") {
            cout << "World: Online | Active Players: " << worldState.getPlayers().size() << "\n";
        }
        else {
            Command cmd = parseCommand(in);
            ExecResult result = execute(cmd, worldState);

            if (result.success) {
                cout << "[Success]: " << result.message << "\n";
            } else {
                cout << "[Exec Error]: " << result.message << "\n";
            }
        }
    }

    InputBuffer buffer;
    buffer.processAndLog(history, "ironclad.log");
}