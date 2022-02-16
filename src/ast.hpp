// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright (C) 2022 Bjarni Dagur Thor Kárason <bjarni@bjarnithor.com>
 */
#pragma once

#include "visitor.hpp"
#include <memory>
#include <string>
#include <vector>

enum class UnaryOperator
{
    None,
    OpStar,
    OpQuestion,
    OpPlus,
};

enum class BinaryOperator
{
    None,
    OpOr,
};

class Node
{
  public:
    virtual ~Node(){};
    virtual void accept(Visitor *visitor) = 0;
};

class LetterNode : public Node
{
  public:
    LetterNode(int dx, int dy, std::string predicate);
    ~LetterNode();
    void accept(Visitor *visitor) override;
    int dx;
    int dy;
    std::string predicate;
};

class CoreWordNode : public Node
{
  public:
    CoreWordNode(Node *childNode);
    ~CoreWordNode();
    void accept(Visitor *visitor) override;
    Node *childNode;
};

class UnaryWordNode : public Node
{
  public:
    UnaryWordNode(Node *childNode);
    ~UnaryWordNode();
    void accept(Visitor *visitor) override;
    Node *childNode;
};

class WordNode : public Node
{
  public:
    WordNode(Node *childNode);
    ~WordNode();
    void accept(Visitor *visitor) override;
    Node *childNode;
};

class SentenceNode : public Node
{
  public:
    SentenceNode();
    ~SentenceNode();
    void accept(Visitor *visitor) override;
    void add_word_node(WordNode *wordNode);
    std::vector<WordNode *> wordNodes;
};

class UnaryOpNode : public Node
{
  public:
    UnaryOpNode(UnaryOperator unaryOperator, Node *childNode);
    ~UnaryOpNode();
    void accept(Visitor *visitor) override;
    UnaryOperator unaryOperator;
    Node *childNode;
};

class BinaryOpNode : public Node
{
  public:
    BinaryOpNode(BinaryOperator binaryOperator, Node *childNodeLHS, Node *childNodeRHS);
    ~BinaryOpNode();
    void accept(Visitor *visitor) override;
    BinaryOperator binaryOperator;
    Node *childNodeLHS;
    Node *childNodeRHS;
};
