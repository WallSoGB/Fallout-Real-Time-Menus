#pragma once

#include "NiPoint3.hpp"

class NiMatrix3 {
public:
	float	m_pEntry[3][3];

	NiMatrix3() {}
	NiMatrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22) noexcept
	{
		m_pEntry[0][0] = m00;
		m_pEntry[0][1] = m10;
		m_pEntry[0][2] = m20;
		m_pEntry[1][0] = m01;
		m_pEntry[1][1] = m11;
		m_pEntry[1][2] = m21;
		m_pEntry[2][0] = m02;
		m_pEntry[2][1] = m12;
		m_pEntry[2][2] = m22;
	}

	__forceinline void SetCol(uint32_t uiCol, float f0, float f1, float f2) noexcept {
		ASSUME_ASSERT(uiCol < 3);
		m_pEntry[0][uiCol] = f0;
		m_pEntry[1][uiCol] = f1;
		m_pEntry[2][uiCol] = f2;
	}
	__forceinline void SetCol(uint32_t uiCol, const NiPoint3& col) noexcept {
		ASSUME_ASSERT(uiCol < 3);
		m_pEntry[0][uiCol] = col.x;
		m_pEntry[1][uiCol] = col.y;
		m_pEntry[2][uiCol] = col.z;
	}
	__forceinline void GetCol(uint32_t uiCol, float& f0, float& f1, float& f2) const noexcept {
		ASSUME_ASSERT(uiCol < 3);
		f0 = m_pEntry[0][uiCol];
		f1 = m_pEntry[1][uiCol];
		f2 = m_pEntry[2][uiCol];
	}

	__forceinline void GetCol(uint32_t uiCol, NiPoint3& col) const noexcept {
		ASSUME_ASSERT(uiCol < 3);
		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];
		col.z = m_pEntry[2][uiCol];
	}

	__forceinline NiPoint3 GetCol(uint32_t uiCol) const noexcept {
		ASSUME_ASSERT(uiCol < 3);
		NiPoint3 col;
		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];
		col.z = m_pEntry[2][uiCol];
		return col;
	}

	NiMatrix3(const NiPoint3& col0, const NiPoint3& col1, const NiPoint3& col2) noexcept {
		SetCol(0, col0);
		SetCol(1, col1);
		SetCol(2, col2);
	}

	__forceinline void GetRow(uint32_t uiRow, NiPoint3& arRow) const noexcept {
		ASSUME_ASSERT(uiRow < 3);
		arRow.x = m_pEntry[uiRow][0];
		arRow.y = m_pEntry[uiRow][1];
		arRow.z = m_pEntry[uiRow][2];
	}

	float* GetRow(uint32_t uiRow) noexcept {
		ASSUME_ASSERT(uiRow < 3);
		return &m_pEntry[uiRow][0];
	}

	__forceinline void SetRow(uint32_t uiRow, float f0, float f1, float f2) noexcept {
		ASSUME_ASSERT(uiRow < 3);
		m_pEntry[uiRow][0] = f0;
		m_pEntry[uiRow][1] = f1;
		m_pEntry[uiRow][2] = f2;
	}

	__forceinline void SetRow(uint32_t uiRow, const NiPoint3& arRow) noexcept {
		ASSUME_ASSERT(uiRow < 3);
		m_pEntry[uiRow][0] = arRow.x;
		m_pEntry[uiRow][1] = arRow.y;
		m_pEntry[uiRow][2] = arRow.z;
	}

	__forceinline float GetEntry(uint32_t uiRow, uint32_t uiCol) const noexcept {
		ASSUME_ASSERT(uiRow < 3);
		ASSUME_ASSERT(uiCol < 3);
		return m_pEntry[uiRow][uiCol];
	}

	__forceinline void SetEntry(uint32_t uiRow, uint32_t uiCol, float fValue) noexcept {
		ASSUME_ASSERT(uiRow < 3);
		ASSUME_ASSERT(uiCol < 3);
		m_pEntry[uiRow][uiCol] = fValue;
	}

	static const NiMatrix3 IDENTITY;
};


ASSERT_SIZE(NiMatrix3, 0x24);

typedef NiMatrix3 NiMatrix33;