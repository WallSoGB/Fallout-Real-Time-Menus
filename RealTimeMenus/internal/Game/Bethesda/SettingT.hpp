#pragma once

#include "Setting.hpp"

template <typename COLLECTION>
class SettingT : public Setting {
public:
	SettingT(const char* apName, const char* apValue);
	SettingT(const char* apName, int32_t aiValue);
	SettingT(const char* apName, uint32_t auValue);
	SettingT(const char* apName, float afValue);
	SettingT(const char* apName, bool abValue);
	virtual ~SettingT() override;

	static COLLECTION* GetCollection();

private:
	static COLLECTION* pCollection;

	static void InitCollection();
};