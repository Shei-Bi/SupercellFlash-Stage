#pragma once

#include <core/io/buffer_stream.h>
#include <core/io/file_stream.h>
#include <compression/compression.h>

#include "core/time/timer.h"
#include <stdio.h>
#include <core/crypto/md5.h>

#include "SWFString.hpp"

namespace sc
{
	namespace flash {
		using namespace sc::compression::flash;
		class SWFStream : public BufferStream
		{
		public:
			SWFStream() {};
			SWFStream(const SWFStream&) {};
			SWFStream& operator=(const SWFStream&) { return *this; };

		public:
#pragma region File IO
		public:
			void open_file(const std::filesystem::path& path)
			{
				clear();
				// sc::Timer operation_timer;

				InputFileStream file(path);
				// int len = file.length() - file.position();
				// std::uint8_t* rest = new std::uint8_t[len];
				// file.read(rest, len);
				// (*this).write(rest, len);
				// delete[] rest;
				Decompressor::decompress(file, *this);
				// printf("Decompressing SC %s took %d ms\n", path.filename().string().c_str(), (int)operation_timer.elapsed());

				seek(0);
			}

			void save_file(const std::filesystem::path& path, Signature signature)
			{
				OutputFileStream file(path);
				if (static_cast<int>(signature) == -1) {
					seek(0);
					file.write_unsigned_short(SC_MAGIC);
					file.write_int(2, Endian::Big);
					// hash MD5
					{
						MD5::md5 md_ctx;
						std::uint8_t hash[MD5::HASH_LENGTH];

						md_ctx.update((std::uint8_t*)data(), length());

						md_ctx.final(hash);

						file.write_unsigned_int((uint32_t)MD5::HASH_LENGTH, Endian::Big);
						file.write(&hash, MD5::HASH_LENGTH);
					}
					file.write(data(), length());
					clear();
					return;
				}

				Compressor::Context context;
				context.signature = signature;

				seek(0);
				Compressor::compress(*this, file, context);
				clear();
			}
#pragma endregion

#pragma region Writing Functions
		public:
			void inline write_string(const SWFString& string)
			{
				uint8_t string_size = string.length();
				if (string_size)
				{
					write_unsigned_byte(string_size);
					write(string.data(), string_size);
				}
				else
				{
					write_unsigned_byte(0xFF);
				}
			}

			void inline write_twip(float twip) {
				write_int((int)(twip / 0.05f));
			}

			size_t inline write_tag_header(uint8_t tag)
			{
				write_unsigned_byte(tag);
				write_int(-1);
				return position();
			}

			void inline write_tag_final(size_t tag_start)
			{
				int* tag_length = (int*)((uint8_t*)data() + (tag_start - 4));
				*tag_length = static_cast<int>(position() - tag_start);
			}

			void inline write_tag_flag(uint8_t tag)
			{
				write_unsigned_byte(tag);
				write_int(0);
			}

#pragma endregion

#pragma region Reading Functions
		public:
			void inline read_string(SWFString& string)
			{
				uint8_t string_size = read_unsigned_byte();

				if (string_size != 0xFF)
				{
					string.resize(string_size);
					read(string.data(), string_size);
					return;
				}

				string.resize(0);
			}

			float inline read_twip()
			{
				return (float)read_int() * 0.05f;
			}
#pragma endregion
		};
	}
}