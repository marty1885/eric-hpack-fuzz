#include <cstdint>
#include <stdint.h>
#include <stddef.h>
#include <vector>
#include "hpack.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    EricHpack::Hpack hpack;
    std::vector<EricHpack::KeyValPair> header;

    const uint8_t* begin = data;
    const uint8_t* end = data + size;

    int nframes = 0;
    while(begin != end) {
        size_t len = static_cast<size_t>(*begin);
        if (begin + len > end) {
            len = end - begin;
        }
        // 0 length not allowed explicitly by the HpRBuffer interface
        if(len == 0) {
            break;
        }

        std::unique_ptr<EricHpack::HpRBuffer> rbuffer = EricHpack::Hpack::MakeHpRBuffer(begin, len);
        begin += len;
        if(!rbuffer) {
            continue;
        }
        hpack.Decoder(*rbuffer, header);
        nframes++;
        
    }
    __sanitizer_cov_trace_cmp8(nframes, nframes);

    return 0;
}
