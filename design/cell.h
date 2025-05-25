#pragma once

#include "common.h"
#include "formula.h"

#include <memory>
#include <unordered_set>

class Impl;

class Cell;

namespace RawCellImpl{
//данный интерфейс используется для получения доступа к ячейке без необходимости ее приведения к типу Cell*
//из типа CellInterface* с помощью dynamic_cast. Интерфейс вводится для избегания круговой зависимости между файлами
//cell.h и sheet.h. Логично, что Sheet может зависеть от Cell, а не только от CellInerface
class GetRawCellInterface {
    virtual const Cell* GetRawCell(Position) const = 0;
};
};

class Cell : public CellInterface {
public:
    Cell(SheetInterface& sheet);
    ~Cell();

    //Алгоритм работы Set описан в new_cell.pdf
    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;\
    
    //возврат ячеек, задействованных в формуле. Список ячеек будет храниться внутри класса FormulaAST
    std::vector<Position> GetReferencedCells() const override;
private:
    //Метод, сбрасывающий кэш ячейки.
    //Метод должен сбросить кэш как данной ячейки через интерфейс Impl, так и всех зависимых ячеек в контейнере dependent_cells_, вызвав в них такой же методы
    void CacheReset() const; 

    //Этот метод реализует рекурсивный обход графа зависимостей ячеек в глубину.
    //Алгоритм работы указан в cyclic_dependencies.pdf
    bool HasCircularDependecies(Cell* start_cell) const;

    //Метод, добавляющий зависящую от данной ячейку. Предполагается вызов только объектами того же класса, поэтому запривачен
    void AddDependentCell(Cell* pos);
    
    //Метод, убирающий зависящую от данной ячейку. 
    void DeleteDependentCell(Cell* pos);

    SheetInterface& sheet_;
    std::unique_ptr<Impl> cell_impl_;

    //Множество ячеек, сслающихся на ячейку. Хранится в виде указателей, так как сама ячейка не знает
    //о своей позиции в таблице. Так проще отслеживать зависимости
    std::unordered_set<Cell*> dependent_cells_;
    //Множество ячеек, на которые ссылается данная ячейка
    std::unordered_set<Cell*> parent_cells_;
};