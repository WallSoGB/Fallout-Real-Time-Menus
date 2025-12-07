#include "TESPackage.hpp"

// GAME - 0x672DD0
PackageLocation* TESPackage::GetPackageSecondLocation() const {
    return ThisCall<PackageLocation*>(0x672DD0, this);
}
