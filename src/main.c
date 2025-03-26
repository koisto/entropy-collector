
#include <stdio.h>
#include <stdint.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#define ENTROPY_BITS        128


struct entropy_colletor {
    uint32_t bits_collected;
    uint32_t n_0_bits;
    uint32_t n_1_bits;
    uint8_t rand_bits[ENTROPY_BITS / 8];
};

void init_entropy_collector (struct entropy_colletor * ec)
{
    ec->bits_collected = 0;
    ec->n_0_bits = 0;
    ec->n_1_bits = 0;
    for (int i = 0; i < (ENTROPY_BITS / 8); i++)
    {
        ec->rand_bits[i] = 0;
    }
}

void print_entrop_collector_state (struct entropy_colletor * ec)
{
    printf("%4u,%4u,%4u, ", ec->bits_collected, ec->n_0_bits, ec->n_1_bits);
    for (int i = 0; i < (ENTROPY_BITS / 8); i++)
    {
        printf("%02x,", ec->rand_bits[i]);
    }
}

void entropy_collector_es_mouse_move (struct entropy_colletor * ec, const EmscriptenMouseEvent *mouseEvent)
{
    int next_bit = 0;

    if (ec->bits_collected < ENTROPY_BITS)
    {
        switch (ec->bits_collected % 3)
        {
            case 0:
                next_bit = ((int)mouseEvent->timestamp) & 1;
                break;
            case 1:
                next_bit = mouseEvent->targetX & 1;
                break;
            case 2:
                next_bit = mouseEvent->targetY & 1;
                break;

        }

        // gather stats
        next_bit ? ec->n_1_bits++ : ec->n_0_bits++;

        // workout offset to which byte in the rand_bits array
        int word_index = ec->bits_collected / 8;

        // workout offset to which bit in the word
        int bit_index = ec->bits_collected - (word_index * 8);

        // stuff the bit in
        ec->rand_bits[word_index] |= next_bit << bit_index;

        ec->bits_collected++;

        printf("%f,%3d,%3d,%2d,%2d,%1d, ", mouseEvent->timestamp, mouseEvent->targetX, mouseEvent->targetY, word_index, bit_index, next_bit);
        print_entrop_collector_state(ec);
        printf("\n");
    }
}

void do_main_loop (void)
{

}

bool mouse_cb(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
    {
        struct entropy_colletor * myUd = (struct entropy_colletor *)userData;

        entropy_collector_es_mouse_move(myUd, mouseEvent);
    }
    return true;
}

int main (void)
{
    struct entropy_colletor myUd;
    init_entropy_collector(&myUd);

    emscripten_set_mousemove_callback("#canvas", &myUd, 1 , mouse_cb);

    emscripten_set_main_loop(do_main_loop, 0, true);



    return 0;
}
