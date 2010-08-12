#include <dlfcn.h>
#include <alsa/asoundlib.h>
#include <stdlib.h>

#ifndef RTLD_NOW
//#define RTLD_NOW ((void*)2)
#error "RTLD_NOW not defined. :v"
#endif

int (*_snd_pcm_open)(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode);

static const char* correct_device = NULL;

static void init_lib(void)
{
   static int libinit = 0;

   correct_device = getenv("ALSA_DEVICE");
   if ( correct_device && strlen(correct_device) == 0 )
   {
      correct_device = NULL;
   }

   if (!libinit)
   {
      void *handle = dlopen("libasound.so", RTLD_NOW);
      if ( handle )
      {
         _snd_pcm_open = dlsym(handle, "snd_pcm_open");
         if ( !_snd_pcm_open )
            fprintf(stderr, "ERROR: ALSA_REROUTE. Could not load libasound, all calls to snd_pcm_open() will fail!\n");
      }
   }
   libinit++;
}


int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode)
{
   init_lib();

   const char *new_dev = (correct_device) ? correct_device : name;

   fprintf(stderr, "ALSA_REROUTE: Got device: \"%s\" Using device: \"%s\"\n", name, new_dev);

   if ( _snd_pcm_open == NULL )
      return -1;

   return _snd_pcm_open(pcm, new_dev, stream, mode);
}
