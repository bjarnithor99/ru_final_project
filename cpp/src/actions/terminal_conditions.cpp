// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2022 Bjarni Dagur Thor Kárason <bjarni@bjarnithor.com>
 */
#include "terminal_conditions.hpp"

NoMovesLeft::NoMovesLeft() {}
NoMovesLeft::~NoMovesLeft() {}
bool NoMovesLeft::operator()(Environment *environment) {
    if (environment->variables.n_moves_found == 0) {
        environment->variables.black_score = 0;
        environment->variables.white_score = 0;
        environment->variables.game_over = true;
        return true;
    }
    return false;
}
std::string NoMovesLeft::get_name() const {
    return "nomovesleft";
}

BlackReachedEnd::BlackReachedEnd() {}
BlackReachedEnd::~BlackReachedEnd() {}
bool BlackReachedEnd::operator()(Environment *environment) {
    for (size_t i = 0; i < environment->board[0].size(); i++) {
        if (environment->board[environment->board.size() - 1][i].player == "black") {
            environment->variables.black_score = 1;
            environment->variables.white_score = -1;
            environment->variables.game_over = true;
            return true;
        }
    }
    return false;
}
std::string BlackReachedEnd::get_name() const {
    return "blackreachedend";
}

WhiteReachedEnd::WhiteReachedEnd() {}
WhiteReachedEnd::~WhiteReachedEnd() {}
bool WhiteReachedEnd::operator()(Environment *environment) {
    for (size_t i = 0; i < environment->board[0].size(); i++) {
        if (environment->board[0][i].player == "white") {
            environment->variables.black_score = -1;
            environment->variables.white_score = 1;
            environment->variables.game_over = true;
            return true;
        }
    }
    return false;
}
std::string WhiteReachedEnd::get_name() const {
    return "whitereachedend";
}

std::map<std::string, std::shared_ptr<TerminalCondition>> TerminalConditions::terminal_conditions = {
    {"nomovesleft", std::make_shared<NoMovesLeft>()},
    {"blackreachedend", std::make_shared<BlackReachedEnd>()},
    {"whitereachedend", std::make_shared<WhiteReachedEnd>()},
};
