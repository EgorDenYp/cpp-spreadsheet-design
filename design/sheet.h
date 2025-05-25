#pragma once

#include "cell.h"
#include "common.h"

#include <functional>

class Sheet : public SheetInterface, public RawCellImpl::GetRawCellInterface {
public:
    using CellPtr = std::unique_ptr<Cell>;
    using ConstCellPtr = std::unique_ptr<const Cell>;
    using CellMatrix = std::vector<std::vector<CellPtr>>;

    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    //метод интерфейса GetRawCellInterface
    const Cell* GetRawCell(Position) const override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами

private:
	  CellMatrix cell_matrix_;
    Size printable_area_ = {0, 0};

    bool IsValidInSizePos(Position pos) const;
    void MatrixResize(int new_height, int new_with);
    CellPtr& AccessCell(Position pos);
    const CellPtr& AccessCell(Position pos) const;
    void RecalculatePrintArea();

};