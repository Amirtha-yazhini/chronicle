#include <iostream>
#include <string>
#include "console.hpp"
#include "Engine.hpp"

using namespace std;

void console(int argc, char* argv[]) {
    string worldFile = "ironhold.world";
    if (argc > 1) {
        worldFile = argv[1];
    }

    WorldState worldState(worldFile);
    string in;

    while (true) {
        cout << "ironhold> ";
        if (!getline(cin, in) || in.empty()) continue;

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
            worldState.close();
            break;
        }
        else if (in == ".status") {
            cout << "World: online — " << worldState.getFilename() 
                 << " (" << worldState.getNumEntities() << " entities, " 
                 << worldState.getPageCount() << " pages)\n";
        }
        else {
            Command cmd = parseCommand(in);
            ExecResult result = execute(cmd, worldState);

            if (!result.message.empty()) {
                if (result.success) {
                    cout << result.message << "\n";
                } else {
                    cout << "[Exec Error]: " << result.message << "\n";
                }
            }
        }
    }
}