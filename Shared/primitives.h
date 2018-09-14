#pragma once

#ifndef PRIMITIVES_H__
#define PRIMITIVES_H__
#include "stdafx.h"

namespace primitives {
	typedef uint8_t byte;
	typedef unsigned short word;
	struct bytes {
		byte* data;
		unsigned int size;
		bytes(size_t);
		bytes() {};
		//void operator=(int);
		void operator=(bytes);
		byte& operator[](int i);
		friend std::istream& operator>>(std::istream&, bytes&);
		friend std::ostream& operator<<(std::ostream&, const bytes&);
	};
}
#endif // !PRIMITIVES_H__