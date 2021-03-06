/*
Copyright (c) 2009, ImageShack Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the ImageShack nor the names of its contributors may be
  used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef DEFINES_H
#define DEFINES_H

// Define this to test using special URL for new video processing
// pipeline
#undef YUV_TEST

#define BOUNDARY "UPLOADERBOUNDARY"

#define NOT_LOGGEDIN_IMAGE_SIZE_LIMIT 5*1024*1024 //5 MB
#define LOGGEDIN_IMAGE_SIZE_LIMIT 10*1024*1024 // 10 MB

#define UPDATE_HOSTNAME "toolbar.imageshack.us"
#define UPDATE_PATH "/windows-uploader/update.xml"

#define UPLOAD_HOSTNAME "imageshack.us"
#define UPLOAD_PATH "/upload_api.php"

#ifdef YUV_TEST
# define VIDEO_UPLOAD_HOSTNAME "render1.imageshack.us"
# define VIDEO_UPLOAD_PATH "/upload_api2.php"
#else 
# define VIDEO_UPLOAD_HOSTNAME "render.imageshack.us"
# define VIDEO_UPLOAD_PATH "/upload_api.php"
#endif

#define CHUNKED_VIDEO_UPLOAD_HOSTNAME "render.imageshack.us"
#define CHUNKED_VIDEO_UPLOAD_PATH "/renderapi"

//#define CHUNKED_VIDEO_UPLOAD_HOSTNAME "localhost"
//#define CHUNKED_VIDEO_UPLOAD_PATH "/renderapi"

#define LOGIN_HOSTNAME "www.imageshack.us"
#define LOGIN_PATH "/auth.php"

#define GALLERY_HOSTNAME "imageshack.us"
#define GALLERY_PATH "/gallery_api.php"

#define TWITTER_HOST "yfrog.com"
#define TWITTER_PATH "/api/uploadAndPost"

#define LOCALIZATION_URL "http://code.google.com/p/imageshack-uploader/wiki/TranslationInstructions"

//old localization_url
//"http://toolbar.imageshack.us/windows-uploader/localization.php"

#define SUPPORT_URL "http://reg.imageshack.us/content.php?page=email&q=customer"

#endif // DEFINES_H
