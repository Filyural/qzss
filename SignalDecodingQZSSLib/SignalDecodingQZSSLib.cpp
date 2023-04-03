// SignalDecodingQZSSLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"

namespace Bits {
	BitContainer::BitContainer(int size) : m_data(size) {}

	bool BitContainer::get(int index) const{
		return m_data[index];
	}

	void BitContainer::set(int index, bool value) {
		m_data[index] = value;
	}

	void BitContainer::toggle(int index) {
		m_data[index] = !m_data[index];
	}

	void BitContainer::clear() {
		m_data.clear();
	}

	int BitContainer::size() const {
		return m_data.size();
	}
}