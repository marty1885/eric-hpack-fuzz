#include <stdint.h>
#include <stddef.h>
#include <vector>
#include "hpack.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    EricHpack::Hpack hpack;
    std::vector<EricHpack::KeyValPair> header;

    // Create the buffer from fuzzer input
    // The library expects unsigned char*, so we cast the uint8_t*
    std::unique_ptr<EricHpack::HpRBuffer> rbuffer = EricHpack::Hpack::MakeHpRBuffer(data, size);

    if (rbuffer) {
        // Run the decoder
        // We catch exceptions just in case, though the library seems to use return codes.
        // If it throws, we want the fuzzer to find it (unless it's a known safe exception).
        // Based on the header, it returns HpackStatus, so likely no exceptions.
        hpack.Decoder(*rbuffer, header);
    }

    return 0;
}
