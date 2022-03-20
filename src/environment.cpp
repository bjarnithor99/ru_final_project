// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2022 Bjarni Dagur Thor Kárason <bjarni@bjarnithor.com>
 */
// This include statement is a hack. The Predicate class needs to access member
// variables of the Environment class, so predicates.hpp includes
// environment.hpp. However, the Environment class also needs to access member
// functions of the Predicates class. To avoid circular includes, predicates.hpp
// includes Environment, dfa.hpp and nfa.hpp (which store pointers to Predicate)
// forward declare the Predicate class, and this compilation unit includes
// predicates.hpp, effectively including environment.hpp as well.
#include "predicates.hpp"
#include "side_effects.hpp"

#define COUTRESET "\033[0m"
#define COUTRED "\033[1m\033[31m"
#define COUTBLUE "\033[1m\033[34m"

Cell::Cell() {}
Cell::Cell(int x, int y, std::string piece, std::string player, DFAState *state)
    : x(x), y(y), piece(piece), player(player), state(state) {}
Cell::~Cell() {}

Step::Step(int x, int y, std::shared_ptr<SideEffect> side_effect) : x(x), y(y), side_effect(side_effect) {}
Step::~Step() {}

Environment::Environment(int board_size_x, int board_size_y) : board_size_x(board_size_x), board_size_y(board_size_y) {}
Environment::~Environment() {}

bool Environment::contains_cell(int x, int y) {
    return 0 <= x && x < board_size_x && 0 <= y && y < board_size_y;
}

int Environment::set_cell(int x, int y, Cell *cell) {
    if (!contains_cell(x, y))
        return -1;
    board[x][y].x = cell->x;
    board[x][y].y = cell->y;
    board[x][y].piece = cell->piece;
    board[x][y].player = cell->player;
    board[x][y].state = cell->state;
    return 0;
}

Cell *Environment::get_cell(int x, int y) {
    if (!contains_cell(x, y))
        return nullptr;
    return &board[x][y];
}

void Environment::generate_moves(std::string player) {
    current_player = player;
    found_moves.clear();
    for (int i = 0; i < board_size_x; i++) {
        for (int j = 0; j < board_size_y; j++) {
            if (board[i][j].player == player) {
                candidate_move.clear();
                candidate_move.push_back(Step(i, j, SideEffects::get_side_effect["default"]));
                generate_moves(board[i][j].state, i, j);
            }
        }
    }
}

void Environment::generate_moves(DFAState *state, int x, int y) {
    if (state->is_accepting)
        found_moves.push_back(candidate_move);
    for (auto &p : state->transition) {
        const DFAInput &input = p.first;
        int next_x = x - input.dy;
        int next_y = y + input.dx;
        if (!contains_cell(next_x, next_y))
            continue;
        if (!(*input.predicate)(this, next_x, next_y))
            continue;
        candidate_move.push_back(Step(next_x, next_y, input.side_effect));
        generate_moves(p.second, next_x, next_y);
        candidate_move.pop_back();
    }
}

void Environment::execute_move(const std::vector<Step> &move) {
    int n_steps = move.size();
    for (int i = 1; i < n_steps; i++) {
        int old_x = move[i - 1].x;
        int old_y = move[i - 1].y;
        int new_x = move[i].x;
        int new_y = move[i].y;
        (*(move[i].side_effect))(this, old_x, old_y, new_x, new_y);
    }
}

void Environment::print() {
    for (int i = 0; i < board_size_x; i++) {
        if (i != 0)
            std::cout << "\n";
        for (int j = 0; j < board_size_y; j++) {
            if (j != 0)
                std::cout << " ";
            if (board[i][j].player == "white")
                std::cout << COUTBLUE << std::setw(8) << board[i][j].piece << COUTRESET;
            else if (board[i][j].player == "black")
                std::cout << COUTRED << std::setw(8) << board[i][j].piece << COUTRESET;
            else
                std::cout << std::setw(8) << board[i][j].piece;
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}
