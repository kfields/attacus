# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

set(ASTC_CODEC_ROOT ${BIMG_ROOT}/3rdparty/astc-codec)

set(_ ${ASTC_CODEC_ROOT}/src/decoder)

add_library(footprint ${_}/footprint.cc)
target_link_libraries(footprint base)
target_include_directories(footprint PRIVATE ${ASTC_CODEC_ROOT})

add_library(astc_utils
${_}/astc_file.cc
${_}/endpoint_codec.cc
${_}/integer_sequence_codec.cc
${_}/intermediate_astc_block.cc
${_}/logical_astc_block.cc
${_}/partition.cc
${_}/physical_astc_block.cc
${_}/quantization.cc
${_}/weight_infill.cc)

target_link_libraries(astc_utils PRIVATE base footprint)
target_include_directories(astc_utils PRIVATE ${ASTC_CODEC_ROOT})

add_library(astc-codec ${_}/codec.cc)
target_link_libraries(astc-codec PRIVATE astc_utils)
target_include_directories(astc-codec PUBLIC ${ASTC_CODEC_ROOT})
target_include_directories(astc-codec PRIVATE ${ASTC_CODEC_ROOT})
