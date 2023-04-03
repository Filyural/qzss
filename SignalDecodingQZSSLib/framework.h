#pragma once

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows


class BitContainer {
public:
	BitContainer(size_t initialSize);
private:
	std::vector<int> bits;
	size_t size;

public:
	bool get(size_t index) const;

	void set(size_t index, bool value);

	void add(bool value);

	size_t getMemoryUsage() const;
};