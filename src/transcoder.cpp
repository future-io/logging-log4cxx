/*
 * Copyright 2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/helpers/pool.h>
#include <stdlib.h>

using namespace log4cxx;
using namespace log4cxx::helpers;



/**
*   Appends an external string to an
*     internal string.
*/
#if defined(LOG4CXX_LOGCHAR_IS_WCHAR)


#if !defined(_GCC_VER_)
namespace log4cxx {
      struct mbstate_t {};
      size_t mbsnrtowcs(wchar_t *dest, const char **src,
          size_t srclenin, size_t destlenin, mbstate_t *ps) {
          size_t destlen = destlenin;
          size_t srclen = srclenin;
          size_t mblen = -1;
          while(srclen > 0 && destlen > 0) {
            mblen = mbtowc(dest, *src, srclen);
            if (mblen <= 0) break;
            *src += mblen;
            *dest++;
            destlen--;
            srclen -= mblen;
          }
          if (mblen < 0) {
              return mblen;
          }
          return destlenin - destlen;
      }

      size_t wcsnrtombs(char *dest, const wchar_t **src, size_t srclenin,
          size_t destlenin, mbstate_t *ps) {
          size_t destlen = destlenin;
          size_t srclen = srclenin;
          size_t mblen = -1;
          char buf[12];
          while(srclen > 0 && destlen > 0) {
            mblen = wctomb(buf, **src);
            if (mblen <= 0) break;
            *src++;
            memcpy(dest, buf, mblen);
            *dest += mblen;
            destlen -= mblen;
            srclen--;
          }
          if (mblen < 0) {
              return mblen;
          }
          return destlenin - destlen;
      }
}
#endif



void Transcoder::decode(const char* src, size_t len, LogString& dst) {
  wchar_t buf[BUFSIZE];
  mbstate_t ps;
  size_t inRemaining = len;
  for(const char* in = src;
     in < src + len && in != NULL;) {
     size_t rv = mbsnrtowcs(buf, &in, len - (in - src), BUFSIZE, &ps);
     if (rv > (size_t) 0) {
        dst.append(buf, rv);
     }
     //
     //   invalid sequence, add a substitution character and move on
     //
     if (rv < 0) {
       dst.append(1, SUBSTITUTION_WCHAR);
       in++;
     }
  }
}

void Transcoder::decode(const wchar_t* src, size_t len, LogString& dst) {
  dst.append(src, len);
}

void Transcoder::encode(const LogString& src, std::string& dst) {
  char buf[BUFSIZE];
  mbstate_t ps;
  const wchar_t* pSrc = src.data();
  size_t srcLen = src.length();
  for(const wchar_t* in = pSrc;
      in < pSrc + srcLen && in != NULL;) {
        size_t rv = wcsnrtombs(buf, &in, srcLen - (in - pSrc), BUFSIZE, &ps);
        if (rv > (size_t) 0) {
          dst.append(buf, rv);
        }
        if (rv < 0) {
          dst.append(1, SUBSTITUTION_CHAR);
          in++;
        }
  }
}

void Transcoder::encode(const LogString& src, std::wstring& dst) {
  dst.append(src);
}

#endif


