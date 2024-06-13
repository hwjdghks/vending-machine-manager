#include "Info.hpp"

Info::Info(int id) : _id(id)
{
    for (int i = 0; i < 6; ++i) {
        _label[i] = ""; // 또는 다른 초기화 방법
        _price[i] = 0;   // 예시로 정수형 배열도 초기화
        _stock[i] = 0;
        _salesTotal[i] = 0;
    }
}

bool Info::operator>(const Info &rhs) const
{
    return this->_id > rhs._id;
}

bool Info::operator<(const Info &rhs) const
{
    return this->_id < rhs._id;
}

bool Info::operator==(const Info &rhs) const
{
    return this->_id == rhs._id;
}

std::ostream &operator<<(std::ostream &os, Info &ref)
{
    int total = 0;

    for (int i = 0; i < 6; i++) {
        os << "[ " << ref._label[i] << " 상품 정보 ]" << '\n'
        << "현재 가격: " << ref._price[i] << "원" << '\n'
        << "현재 재고: " << ref._stock[i] << "개" << '\n'
        << "해당 상품 누적 매출: " << ref._salesTotal[i] << "원" << '\n'
        << '\n';
        total += ref._salesTotal[i];
    }
    os << "ID " << ref._id << "번 자판기 총 매출: " << total << "원" << '\n';
	return os;
}
