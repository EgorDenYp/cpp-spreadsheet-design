#pragma once

#include "FormulaLexer.h"
#include "common.h"

#include <forward_list>
#include <functional>
#include <stdexcept>

namespace ASTImpl {
class Expr;
}

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class FormulaAST {
public:
    explicit FormulaAST(std::unique_ptr<ASTImpl::Expr> root_expr);
    FormulaAST(FormulaAST&&) = default;
    FormulaAST& operator=(FormulaAST&&) = default;
    ~FormulaAST();

    //Теперь в метод Execute необходимо передавать ссылку на таблицу для расчета формул,
    //содержащих индексы ячеек
    double Execute(SheetInterface& sheet) const;

    //метод, позволяющий получить доступ к ячейкам таблицы с помощью Callback функций
    double Execute(std::function<double(Position)>) const;

    void Print(std::ostream& out) const;
    void PrintFormula(std::ostream& out) const;

    //метод, который будет возвращать вектор встреченных в формуле ячеек
    //вектор будет отсортирован по возрастанию
    std::vector<Position> GetInFormulaCells() const;
private:
    std::unique_ptr<ASTImpl::Expr> root_expr_;
    //Поле, сохраняющее все встреченные ячейки внутрь себя (по условию задания)
    std::forward_list<Position> cells_;
};

FormulaAST ParseFormulaAST(std::istream& in);
FormulaAST ParseFormulaAST(const std::string& in_str);