// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
// Copyright (c) 2013, Christian Kellner <kellner@bio.lmu.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/NDArray.hpp>

namespace nix {


NDArray::NDArray(DataType dtype, NDSize dims) : dataType(dtype), extends(dims) {
    allocate_space();
}


void NDArray::allocate_space() {
    size_t type_size = data_type_to_size(dataType);
	ndsize_t bytes = extends.nelms() * type_size;
	size_t alloc_size = check::fits_in_size_t(bytes, "Cannot allocate storage (exceeds memory)");
    dstore.resize(alloc_size);

    calc_strides();
}


void NDArray::resize(const NDSize &new_size) {
    extends = new_size;
    allocate_space();
}


void NDArray::calc_strides() {
    size_t _rank = rank();

    strides = NDSize(_rank, 1);

    for (size_t i = 1; i < _rank; i++) {
        size_t lst = _rank - i;
        size_t cur = lst - 1;
        strides[cur] = strides[lst] * extends[lst];
    }
}


size_t NDArray::sub2index(const NDSize &sub) const {
    ndsize_t pos = strides.dot(sub);
    size_t idx = check::fits_in_size_t(pos, "index does not fit into memory");
    return idx;
}

} // namespace nix
