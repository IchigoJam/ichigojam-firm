/*
 *  player.ino - Simple sound player example application
 *  Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <SDHCI.h>
#include <Audio.h>
#include <File.h>

SDClass theSD;
AudioClass* theAudio = NULL;


bool ErrEnd = false;

/**
 * @brief Audio attention callback
 *
 * When audio internal error occurs, this function will be called back.
 */

static void audio_attention_cb(const ErrorAttentionParam *atprm) {
  puts("Attention!");
  
  if (atprm->error_code >= AS_ATTENTION_CODE_WARNING) {
    ErrEnd = true;
  }
}

void audioplayer_init2() {
  // start audio system
  theAudio = AudioClass::getInstance();

  theAudio->begin(audio_attention_cb);

  puts("initialization Audio Library");

  /* Set clock mode to normal */
  theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);

  /* Set output device to speaker with first argument.
   * If you want to change the output device to I2S,
   * specify "AS_SETPLAYER_OUTPUTDEVICE_I2SOUTPUT" as an argument.
   * Set speaker driver mode to LineOut with second argument.
   * If you want to change the speaker driver mode to other,
   * specify "AS_SP_DRV_MODE_1DRIVER" or "AS_SP_DRV_MODE_2DRIVER" or "AS_SP_DRV_MODE_4DRIVER"
   * as an argument.
   */
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, AS_SP_DRV_MODE_LINEOUT);

  /*
   * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect"
   * Search for MP3 decoder in "/mnt/sd0/BIN" directory
   */
  err_t err = theAudio->initPlayer(AudioClass::Player0, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_STEREO);

  /* Verify player initialize */
  if (err != AUDIOLIB_ECODE_OK) {
      printf("Player0 initialize error\n");
      //exit(1);
  }
}

void audioplayer_init() {
  /* Initialize SD */
  /*
  while (!theSD.begin()) {
    Serial.println("Insert SD card.");
  }
  */
  while (!theSD.begin()) {
    Serial.println("Insert SD card.");
  }

  audioplayer_init2();
}

File myFile = NULL;

/**
 * @brief Setup audio player to play mp3 file
 *
 * Set clock mode to normal <br>
 * Set output device to speaker <br>
 * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect" <br>
 * System directory "/mnt/sd0/BIN" will be searched for MP3 decoder (MP3DEC file)
 * Open "Sound.mp3" file <br>
 * Set master volume to -16.0 dB
 */
void audioplayer_play(const char* mp3fn) {
  int flg = myFile != NULL;
  if (myFile) {
    myFile.close();
    myFile = NULL;
  }
  if (ErrEnd) {
    ErrEnd = false;
    flg = 0;
    // reset
    theAudio->stopPlayer(AudioClass::Player0);
    theAudio->setReadyMode();
    theAudio->end();
    //theAudio = NULL;
    audioplayer_init2();
  }

  /* Open file placed on SD card */
  //myFile = theSD.open("kimigayo.mp3");
  //myFile = theSD.open("45seconds.mp3");
  myFile = theSD.open(mp3fn);
  
  /* Verify file open */
  if (!myFile) {
      printf("File open error\n");
      //exit(1);
  }
  printf("Open! 0x%08lx\n", (uint32_t)myFile);

  /* Send first frames to be decoded */
  int err = theAudio->writeFrames(AudioClass::Player0, myFile);

  if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND)) {
    printf("File Read Error! =%d\n",err);
    myFile.close();
    myFile = NULL;
    return;
    //exit(1);
  }

  if (flg) {
    return;
  }
  /* Main volume set to -16.0 dB */
  puts("Play!");
  theAudio->setVolume(-160);
  theAudio->startPlayer(AudioClass::Player0);
}

/**
 * @brief Play stream
 *
 * Send new frames to decode in a loop until file ends
 */
void audio_loop()
{
  puts("loop!!");

  /* Send new frames to decode in a loop until file ends */
  int err = theAudio->writeFrames(AudioClass::Player0, myFile);

  /*  Tell when player file ends */
  if (err == AUDIOLIB_ECODE_FILEEND)
    {
      printf("Main player File End!\n");
    }

  /* Show error code from player and stop */
  if (err)
    {
      printf("Main player error code: %d\n", err);
      goto stop_player;
    }

  if (ErrEnd)
    {
      printf("Error End\n");
      goto stop_player;
    }

  /* This sleep is adjusted by the time to read the audio stream file.
   * Please adjust in according with the processing contents
   * being processed at the same time by Application.
   *
   * The usleep() function suspends execution of the calling thread for usec
   * microseconds. But the timer resolution depends on the OS system tick time
   * which is 10 milliseconds (10,000 microseconds) by default. Therefore,
   * it will sleep for a longer time than the time requested here.
   */

  //usleep(40000);


  /* Don't go further and continue play */
  return;

stop_player:
  theAudio->stopPlayer(AudioClass::Player0);
  myFile.close();
  theAudio->setReadyMode();
  theAudio->end();
  theAudio = NULL;
  //exit(1);
}
