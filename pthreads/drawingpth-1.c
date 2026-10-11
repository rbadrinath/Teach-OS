#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

// Compile as
// gcc drawingpth-1.c -o drawingpth -lSDL2 -lpthread -lm

#define WIDTH 800
#define HEIGHT 600

// optional argument can be given to specify the number of threads
// default value is specified below
#define DFL_NTHREADS 4

// The frame buffer contains the pixels to be rendered in the window
// Ths global variable is written to by worker threads
// The threads fill independant(non-overlapping) parts
// of this frame buffer with pixel values.
uint32_t framebuffer[WIDTH * HEIGHT];
// Note: this frame buffer matches the windows, which looks like this:
// y=0:    x=0, 1, 2, 3, ... , WIDTH-1
// y=1:
// y=2:
// ...
// y=HEIGHT-1: ... 
// framebuffer is only 1-D, so for given (x,y) is pixel value is p, then
//                compute index i as = y*WIDTH+x
//                then framebuffer[i]=p

// Thread function: Renders a designated horizontal band of the framebuffer
// The x range is 0..WIDTH-1
// The y range is given by the data argument
// Arguments passed to each thread
typedef struct {
    int thread_id;
    int start_y;
    int end_y;
    float time; // This is the animation time step
} ThreadData;
void* render_stripe(void* arg) {

    ThreadData* data = (ThreadData*)arg;
    float t = data->time;

    for (int y = data->start_y; y < data->end_y; y++) {
        for (int x = 0; x < WIDTH; x++) { // WIDTH because full row
			// Index in the framebuffer to insert the pixel value
            int index = y * WIDTH + x;
			// we will now compute the rgb value to put into pixel (x,y)

			// Green wave is wave1
            // Compute wave1 at x at t
            float wave1 = sinf(x * 0.01f + t) * 100.0f + (HEIGHT / 2.0f);
			// Red wave is wave2
            // Compute wave2 at x at t
            float wave2 = cosf(x * 0.02f - t * 1.5f) * 50.0f + (HEIGHT / 2.0f);
			// wave1 or wave2 may not pass through x,y
			// we could leave it alone (only blue background)
			// alternately we will make a thick curve for the waves.. so
			// if y is close enough to wave1 or wave2 we will 
			// put some rgb values for the (x,y) pixel

			// we want for given x,y what should the pixel be? 
            // Calculate distance to the waves to draw a fat line
            float dist1 = fabsf(y - wave1);
            float dist2 = fabsf(y - wave2);

			// calculate rgb values(0-255) based on 
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 150 ; 

            // Add green glow for wave 1
            if (dist1 < 20.0f) {
                g = (uint8_t)255.0;
            }
            // Add red glow for wave 2
            if (dist2 < 15.0f) {
                r = (uint8_t)255.0;
            }

			// Convert the r,g and b values into a single pixel
			// Stored in lowermost 8+8+8 bits
            // Pack pixels as 0xRRGGBB (SDL_PIXELFORMAT_RGB888)
            framebuffer[index] = (r << 16) | (g << 8) | b;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	int real_nthreads=0;
	if (argc>1)
		real_nthreads=atoi(argv[1]);
	if (real_nthreads<=0)
		real_nthreads=DFL_NTHREADS;
	printf("Populating framebuffer with %d threads\n",real_nthreads);


    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Multithreaded SDL2 Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN
    );

    // Create hardware-accelerated renderer mapped to VSYNC
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Create a streaming texture to quickly pump raw pixels from CPU to GPU
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );

    pthread_t threads[real_nthreads];
    ThreadData thread_data[real_nthreads];
    int rows_per_thread = HEIGHT / real_nthreads;

    bool running = true;
    SDL_Event event;
    float animation_time = 0.0f;

    // Main Loop - usual way event driver programs are written with SDL2

	char rc='A'; // just a label  for printf
    while (running) {
        // 1. Process Window Events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // 2. Advance animation frame time
        animation_time += 0.05f;

        // 3. Dispatch Work to Multiple Threads

        for (int i = 0; i < real_nthreads; i++) {
            thread_data[i].thread_id = i;
            thread_data[i].start_y = i * rows_per_thread;
            // Handle edge case if HEIGHT isn't perfectly divisible by real_nthreads
            thread_data[i].end_y = (i == real_nthreads - 1) ? HEIGHT : (i + 1) * rows_per_thread;
            thread_data[i].time = animation_time;

            pthread_create(&threads[i], NULL, render_stripe, (void*)&thread_data[i]);
		
			// printf("%c: Dispatched %d\n",rc,i); // DEBUG
        }
		rc=(rc=='Z')?'A':(rc+1);

        // 4. Wait for all threads to finish computing their section of the buffer
        for (int i = 0; i < real_nthreads; i++) {
            pthread_join(threads[i], NULL);
        }

        // 5. Push the completed Frame Buffer to the GPU Texture
        SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));

        // 6. Draw the Frame
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup Resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
