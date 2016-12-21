#include "src/controller/height_measurement/height_measurement.h"

#define UNDEFINED 576
#define MAX_HEIGHT 2200
#define GROUND_HEIGHT 4000
#define INIT_HEIGHT 4076 // band 8
#define MAX_LENGTH 850
#define MIN_LENGTH 5
#define SUM_IC_WM 3
#define SMALLEST_ABS_ERROR 20000
#define PROTOTYPE_LENGTH 450
#define CUT_OFF_FRONT 8
#define SUM_PUKS 10
#define STOPPED 1
#define INIT_MEASUREMENTS 4

#define TICK_SEC 0
#define TICK_NSEC 20000

static uint32_t last_type;
static bool is_stopped;
static int32_t height_diff;

int timer_cid;
int timer_con;
timer_t timerId;
struct itimerspec timerSpec;
struct sigevent   timerEvent;

static uint32_t const INVALID_PUK_TYPE_ID_E = 12345;
static uint32_t const HM_START_EVENT_ID_E = 54321;
static uint32_t const HM_DONE_EVENT_ID_E = 264864;

uint32_t Height_Measurement::profiles[10][450] = {
		/* 0 iron_core */        { 2607, 2592, 2576, 2568, 2557, 2542, 2541, 2545, 2548, 2550, 2550, 2550, 2553, 2549, 2549, 2550, 2554, 2555, 2555, 2556, 2557, 2555, 2560, 2556, 2559, 2556, 2559, 2559, 2558, 2559, 2559, 2556, 2555, 2555, 2550, 2550, 2548, 2548, 2548, 2549, 2551, 2551, 2552, 2555, 2555, 2554, 2556, 2558, 2557, 2558, 2556, 2557, 2555, 2557, 2555, 2558, 2560, 2563, 2559, 2557, 2560, 2561, 2561, 2561, 2559, 2558, 2558, 2553, 2558, 2559, 2556, 2552, 2555, 2555, 2558, 2555, 2558, 2560, 2562, 2560, 2559, 2562, 2560, 2557, 2559, 2561, 2560, 2559, 2558, 2560, 2562, 2560, 2562, 2560, 2561, 2558, 2554, 2553, 2548, 2545, 2551, 2560, 2565, 2561, 2563, 2564, 2560, 2556, 2553, 2546, 2544, 2543, 2549, 2566, 2607, 2646, 2655, 2652, 2649, 2646, 2646, 2639, 2637, 2642, 2638, 2635, 2636, 2632, 2622, 2614, 2604, 2595, 2589, 2589, 2584, 2579, 2572, 2559, 2550, 2538, 2526, 2514, 2508, 2497, 2755, 3081, 3402, 3647, 3644, 3646, 3650, 3649, 3650, 3648, 3649, 3646, 3649, 3647, 3646, 3645, 3648, 3645, 3642, 3644, 3640, 3640, 3640, 3640, 3641, 3643, 3641, 3641, 3644, 3643, 3644, 3643, 3644, 3641, 3638, 3639, 3640, 3641, 3638, 3640, 3641, 3643, 3641, 3644, 3644, 3645, 3645, 3645, 3648, 3652, 3648, 3650, 3650, 3650, 3649, 3652, 3655, 3655, 3656, 3651, 3653, 3658, 3654, 3653, 3652, 3654, 3657, 3656, 3654, 3657, 3655, 3659, 3657, 3658, 3660, 3657, 3653, 3655, 3655, 3655, 3652, 3653, 3656, 3652, 3652, 3652, 3652, 3651, 3647, 3644, 3647, 3646, 3643, 3642, 3644, 3646, 3639, 3639, 3635, 3636, 3632, 3629, 3632, 3630, 3623, 3627, 3622, 3621, 3619, 3619, 3620, 3617, 3619, 3617, 3620, 3620, 3621, 3621, 3622, 3629, 3623, 3625, 3626, 3625, 3626, 3623, 3624, 3622, 3624, 3623, 3622, 3622, 3622, 3623, 3622, 3629, 3628, 3625, 3625, 3622, 3625, 3624, 3626, 3633, 3641, 3643, 3651, 3654, 3654, 3649, 3642, 3636, 3633, 3619, 3612, 3605, 3608, 3611, 3613, 3621, 3630, 3650, 3667, 3698, 3716, 3746, 3420, 3124, 2791, 2639, 2445, 2452, 2468, 2488, 2497, 2512, 2525, 2539, 2549, 2557, 2568, 2581, 2588, 2591, 2597, 2606, 2612, 2617, 2621, 2632, 2632, 2631, 2629, 2633, 2634, 2633, 2633, 2633, 2633, 2619, 2588, 2566, 2548, 2534, 2526, 2531, 2537, 2539, 2544, 2554, 2572, 2577, 2568, 2558, 2551, 2544, 2536, 2541, 2545, 2551, 2548, 2549, 2554, 2557, 2555, 2555, 2555, 2559, 2559, 2558, 2557, 2562, 2557, 2554, 2555, 2558, 2557, 2554, 2552, 2552, 2551, 2548, 2541, 2547, 2546, 2548, 2544, 2547, 2551, 2546, 2552, 2553, 2553, 2553, 2555, 2553, 2552, 2552, 2550, 2549, 2545, 2545, 2547, 2547, 2547, 2544, 2545, 2545, 2545, 2549, 2545, 2549, 2547, 2551, 2550, 2549, 2551, 2553, 2556, 2551, 2554, 2551, 2555, 2553, 2551, 2558, 2557, 2558, 2558, 2557, 2556, 2555, 2554, 2555, 2555, 2557, 2554, 2558, 2555, 2559, 2557, 2561, 2563, 2556, 2560, 2555 },
		/* 1 iron_core_wmf */    { 2607, 2592, 2578, 2570, 2570, 2557, 2553, 2553, 2552, 2557, 2569, 2578, 2598, 2604, 2611, 2618, 2622, 2626, 2625, 2628, 2628, 2629, 2631, 2629, 2631, 2630, 2634, 2631, 2634, 2637, 2636, 2636, 2637, 2638, 2639, 2639, 2642, 2640, 2639, 2637, 2643, 2645, 2640, 2641, 2645, 2644, 2644, 2642, 2639, 2639, 2641, 2637, 2637, 2636, 2635, 2636, 2638, 2637, 2636, 2638, 2638, 2639, 2643, 2638, 2639, 2641, 2642, 2643, 2640, 2639, 2639, 2635, 2636, 2633, 2631, 2629, 2626, 2629, 2627, 2624, 2628, 2626, 2627, 2629, 2628, 2628, 2627, 2628, 2628, 2623, 2618, 2619, 2613, 2607, 2595, 2587, 2581, 2574, 2567, 2560, 2556, 2566, 2569, 2567, 2560, 2559, 2552, 2547, 2545, 2542, 2545, 2546, 2551, 2566, 2611, 2625, 2642, 2652, 2663, 2664, 2665, 2660, 2645, 2648, 2647, 2645, 2639, 2640, 2633, 2625, 2621, 2616, 2604, 2595, 2593, 2584, 2574, 2566, 2560, 2553, 2536, 2533, 2530, 2519, 2511, 2789, 3068, 3335, 3637, 3642, 3640, 3638, 3639, 3641, 3641, 3642, 3644, 3645, 3641, 3642, 3646, 3647, 3644, 3645, 3647, 3646, 3641, 3646, 3646, 3644, 3642, 3643, 3644, 3641, 3640, 3643, 3644, 3642, 3647, 3648, 3648, 3649, 3649, 3650, 3649, 3652, 3653, 3647, 3649, 3651, 3652, 3652, 3651, 3650, 3654, 3651, 3653, 3655, 3653, 3654, 3652, 3650, 3658, 3660, 3658, 3661, 3660, 3660, 3658, 3657, 3657, 3658, 3654, 3654, 3652, 3654, 3653, 3653, 3654, 3653, 3649, 3652, 3648, 3647, 3644, 3643, 3643, 3639, 3642, 3640, 3642, 3641, 3643, 3643, 3639, 3640, 3637, 3636, 3633, 3632, 3634, 3632, 3631, 3628, 3631, 3630, 3629, 3630, 3628, 3630, 3629, 3631, 3629, 3625, 3626, 3625, 3623, 3625, 3621, 3623, 3620, 3621, 3621, 3619, 3618, 3618, 3618, 3618, 3619, 3620, 3623, 3626, 3624, 3624, 3624, 3622, 3624, 3624, 3625, 3624, 3625, 3626, 3626, 3628, 3629, 3631, 3633, 3639, 3641, 3641, 3643, 3642, 3642, 3642, 3636, 3634, 3628, 3620, 3617, 3611, 3612, 3611, 3616, 3617, 3625, 3634, 3645, 3659, 3677, 3369, 3057, 3058, 3071, 3076, 2780, 2792, 2513, 2492, 2506, 2526, 2536, 2539, 2546, 2548, 2562, 2572, 2577, 2583, 2584, 2588, 2594, 2596, 2608, 2625, 2632, 2638, 2640, 2642, 2645, 2646, 2646, 2646, 2646, 2638, 2626, 2610, 2597, 2590, 2589, 2582, 2566, 2554, 2551, 2548, 2549, 2548, 2546, 2541, 2542, 2549, 2548, 2550, 2552, 2553, 2553, 2547, 2545, 2542, 2539, 2541, 2543, 2546, 2540, 2541, 2542, 2540, 2541, 2545, 2549, 2545, 2547, 2545, 2539, 2539, 2539, 2539, 2540, 2544, 2548, 2547, 2548, 2549, 2549, 2549, 2553, 2554, 2557, 2553, 2553, 2553, 2554, 2553, 2552, 2556, 2553, 2557, 2560, 2562, 2562, 2563, 2563, 2561, 2560, 2562, 2567, 2564, 2567, 2568, 2562, 2561, 2561, 2561, 2563, 2564, 2563, 2564, 2564, 2559, 2559, 2561, 2558, 2557, 2555, 2558, 2554, 2555, 2554, 2553, 2556, 2555, 2555, 2554, 2549, 2551, 2548, 2553, 2551, 2552, 2553, 2553 },
		/* 2 iron_core_wmb */    { 2583, 2573, 2564, 2558, 2550, 2552, 2553, 2555, 2558, 2558, 2561, 2557, 2559, 2560, 2558, 2554, 2555, 2554, 2550, 2550, 2551, 2549, 2546, 2544, 2550, 2557, 2557, 2562, 2561, 2561, 2564, 2561, 2559, 2555, 2554, 2554, 2550, 2550, 2554, 2543, 2544, 2546, 2544, 2543, 2545, 2545, 2545, 2542, 2544, 2548, 2546, 2548, 2551, 2551, 2549, 2551, 2552, 2556, 2556, 2561, 2558, 2561, 2561, 2559, 2563, 2568, 2570, 2570, 2575, 2577, 2576, 2569, 2570, 2567, 2563, 2559, 2560, 2559, 2556, 2559, 2557, 2551, 2558, 2557, 2556, 2556, 2559, 2559, 2553, 2555, 2558, 2555, 2550, 2554, 2555, 2551, 2552, 2550, 2548, 2548, 2554, 2563, 2564, 2564, 2567, 2562, 2560, 2556, 2554, 2547, 2549, 2554, 2566, 2597, 2632, 2658, 2663, 2662, 2670, 2669, 2672, 2673, 2674, 2669, 2664, 2664, 2656, 2648, 2646, 2637, 2630, 2619, 2610, 2597, 2580, 2570, 2563, 2553, 2553, 2554, 2552, 2547, 2548, 2551, 2827, 3086, 3261, 3366, 3354, 3616, 3620, 3618, 3620, 3626, 3625, 3628, 3632, 3633, 3637, 3634, 3644, 3644, 3644, 3648, 3648, 3643, 3645, 3648, 3649, 3646, 3650, 3650, 3646, 3645, 3649, 3649, 3646, 3650, 3653, 3650, 3650, 3654, 3654, 3652, 3654, 3655, 3651, 3650, 3654, 3650, 3648, 3652, 3653, 3651, 3648, 3654, 3648, 3649, 3648, 3648, 3643, 3639, 3642, 3639, 3637, 3639, 3638, 3638, 3639, 3643, 3641, 3641, 3643, 3643, 3639, 3640, 3645, 3642, 3640, 3640, 3644, 3639, 3642, 3643, 3639, 3632, 3632, 3633, 3631, 3631, 3632, 3628, 3628, 3630, 3631, 3629, 3630, 3634, 3633, 3633, 3634, 3630, 3628, 3627, 3629, 3627, 3627, 3631, 3627, 3630, 3626, 3630, 3627, 3629, 3632, 3633, 3630, 3629, 3633, 3632, 3628, 3632, 3627, 3625, 3625, 3627, 3622, 3624, 3626, 3627, 3622, 3625, 3628, 3626, 3624, 3626, 3623, 3618, 3617, 3617, 3616, 3616, 3618, 3617, 3616, 3621, 3624, 3625, 3633, 3634, 3631, 3635, 3639, 3637, 3635, 3632, 3630, 3616, 3611, 3611, 3612, 3614, 3619, 3626, 3633, 3646, 3664, 3681, 3704, 3721, 3404, 3072, 3069, 3078, 3087, 2772, 2777, 2774, 2551, 2520, 2540, 2549, 2556, 2555, 2556, 2560, 2566, 2572, 2571, 2582, 2597, 2608, 2614, 2614, 2621, 2623, 2633, 2638, 2641, 2642, 2641, 2645, 2633, 2613, 2597, 2588, 2581, 2582, 2588, 2590, 2589, 2591, 2594, 2589, 2581, 2585, 2577, 2568, 2562, 2557, 2551, 2546, 2550, 2549, 2558, 2572, 2588, 2595, 2602, 2604, 2603, 2602, 2608, 2616, 2619, 2620, 2624, 2627, 2621, 2623, 2622, 2619, 2621, 2623, 2626, 2627, 2626, 2626, 2629, 2631, 2632, 2634, 2630, 2627, 2627, 2628, 2627, 2627, 2627, 2629, 2628, 2629, 2629, 2633, 2633, 2633, 2634, 2631, 2631, 2631, 2634, 2633, 2632, 2629, 2626, 2627, 2621, 2626, 2624, 2619, 2621, 2618, 2615, 2617, 2620, 2622, 2620, 2624, 2625, 2617, 2622, 2620, 2616, 2616, 2621, 2617, 2618, 2618, 2620, 2624, 2617, 2616, 2610, 2599, 2592, 2588, 2577, 2565, 2547, 2532 },
		/* 3 little_one */       { 2845, 2834, 2836, 2835, 2831, 2833, 2835, 2833, 2833, 2830, 2830, 2834, 2834, 2832, 2831, 2834, 2831, 2832, 2830, 2834, 2830, 2833, 2827, 2826, 2830, 2828, 2831, 2829, 2827, 2822, 2823, 2818, 2821, 2819, 2818, 2818, 2820, 2822, 2820, 2822, 2819, 2825, 2821, 2825, 2821, 2821, 2825, 2823, 2822, 2823, 2828, 2825, 2829, 2819, 2824, 2819, 2820, 2822, 2817, 2819, 2817, 2817, 2815, 2821, 2815, 2818, 2815, 2818, 2814, 2817, 2814, 2818, 2817, 2812, 2818, 2815, 2821, 2818, 2822, 2820, 2823, 2824, 2821, 2822, 2822, 2824, 2823, 2821, 2816, 2818, 2820, 2826, 2818, 2820, 2816, 2814, 2810, 2807, 2812, 2810, 2814, 2815, 2816, 2815, 2819, 2817, 2816, 2817, 2817, 2815, 2819, 2817, 2817, 2822, 2819, 2819, 2821, 2825, 2822, 2819, 2821, 2821, 2822, 2825, 2824, 2823, 2816, 2815, 2818, 2817, 2820, 2817, 2814, 2816, 2821, 2819, 2820, 2820, 2818, 2816, 2820, 2820, 2822, 2824, 2825, 2825, 2820, 2821, 2818, 2823, 2818, 2823, 2826, 2826, 2826, 2826, 2825, 2825, 2826, 2822, 2821, 2821, 2822, 2819, 2821, 2825, 2825, 2828, 2829, 2831, 2831, 2834, 2831, 2829, 2829, 2830, 2825, 2831, 2828, 2828, 2830, 2828, 2828, 2823, 2827, 2820, 2823, 2821, 2822, 2814, 2816, 2817, 2817, 2820, 2816, 2820, 2813, 2814, 2814, 2818, 2816, 2820, 2817, 2816, 2817, 2821, 2820, 2821, 2826, 2823, 2823, 2820, 2826, 2824, 2820, 2828, 2825, 2825, 2824, 2824, 2820, 2822, 2825, 2826, 2825, 2828, 2828, 2824, 2826, 2827, 2828, 2824, 2826, 2824, 2823, 2822, 2821, 2816, 2820, 2822, 2819, 2827, 2828, 2827, 2832, 2833, 2830, 2828, 2829, 2825, 2822, 2822, 2813, 2816, 2817, 2814, 2820, 2820, 2823, 2827, 2828, 2829, 2829, 2828, 2829, 2824, 2820, 2821, 2820, 2817, 2821, 2821, 2823, 2824, 2823, 2820, 2819, 2820, 2818, 2817, 2817, 2817, 2819, 2818, 2824, 2828, 2825, 2824, 2827, 2827, 2822, 2820, 2818, 2819, 2819, 2816, 2818, 2820, 2821, 2820, 2819, 2820, 2823, 2823, 2820, 2820, 2820, 2821, 2819, 2817, 2820, 2819, 2819, 2816, 2814, 2812, 2810, 2811, 2808, 2812, 2811, 2812, 2811, 2814, 2820, 2820, 2819, 2822, 2824, 2821, 2819, 2818, 2819, 2812, 2813, 2814, 2818, 2817, 2820, 2821, 2820, 2822, 2816, 2819, 2816, 2818, 2818, 2814, 2815, 2817, 2816, 2821, 2820, 2818, 2822, 2821, 2817, 2812, 2815, 2810, 2812, 2815, 2818, 2819, 2819, 2819, 2816, 2814, 2814, 2813, 2810, 2813, 2813, 2812, 2813, 2811, 2812, 2809, 2812, 2808, 2814, 2816, 2817, 2820, 2822, 2820, 2816, 2817, 2815, 2814, 2818, 2817, 2819, 2821, 2819, 2820, 2819, 2816, 2816, 2818, 2814, 2816, 2816, 2819, 2815, 2817, 2816, 2816, 2821, 2823, 2824, 2819, 2820, 2818, 2818, 2816, 2819, 2819, 2819, 2818, 2815, 2814, 2815, 2820, 2817, 2823, 2820, 2821, 2819, 2819, 2818, 2818, 2818, 2816, 2819, 2818, 2816, 2812, 2816, 2814, 2815, 2815, 2818, 2824, 2822, 2825, 2821, 2823, 2819, 2824 },
		/* 4 normal */           { 2642, 2629, 2617, 2602, 2588, 2581, 2577, 2566, 2562, 2559, 2557, 2553, 2556, 2554, 2554, 2555, 2554, 2553, 2554, 2553, 2550, 2554, 2552, 2553, 2554, 2557, 2559, 2558, 2558, 2559, 2556, 2557, 2557, 2560, 2556, 2556, 2563, 2559, 2563, 2557, 2558, 2560, 2559, 2561, 2563, 2560, 2557, 2562, 2560, 2560, 2559, 2561, 2559, 2561, 2558, 2557, 2559, 2556, 2560, 2559, 2560, 2559, 2558, 2555, 2557, 2557, 2561, 2559, 2562, 2559, 2560, 2557, 2560, 2557, 2559, 2562, 2558, 2561, 2556, 2556, 2555, 2556, 2554, 2556, 2558, 2559, 2559, 2560, 2560, 2558, 2560, 2557, 2558, 2555, 2559, 2554, 2557, 2555, 2556, 2555, 2556, 2557, 2557, 2557, 2558, 2564, 2560, 2563, 2561, 2561, 2561, 2562, 2559, 2559, 2559, 2558, 2560, 2559, 2559, 2557, 2559, 2554, 2555, 2552, 2555, 2552, 2551, 2554, 2549, 2551, 2550, 2553, 2551, 2555, 2556, 2555, 2554, 2555, 2555, 2552, 2556, 2552, 2554, 2550, 2555, 2554, 2557, 2554, 2559, 2556, 2558, 2559, 2557, 2557, 2559, 2559, 2558, 2559, 2555, 2560, 2557, 2560, 2557, 2560, 2559, 2560, 2559, 2562, 2560, 2555, 2559, 2561, 2561, 2560, 2562, 2558, 2562, 2557, 2559, 2561, 2566, 2561, 2562, 2563, 2557, 2558, 2558, 2557, 2557, 2561, 2557, 2563, 2559, 2559, 2563, 2567, 2563, 2567, 2565, 2567, 2564, 2558, 2563, 2558, 2556, 2553, 2560, 2556, 2559, 2561, 2565, 2563, 2567, 2565, 2569, 2571, 2568, 2576, 2575, 2576, 2573, 2577, 2570, 2575, 2570, 2571, 2570, 2570, 2571, 2573, 2570, 2572, 2575, 2572, 2578, 2574, 2577, 2574, 2581, 2577, 2578, 2579, 2579, 2584, 2588, 2589, 2591, 2594, 2590, 2588, 2585, 2583, 2575, 2567, 2565, 2563, 2559, 2556, 2561, 2557, 2559, 2559, 2560, 2551, 2561, 2560, 2561, 2561, 2566, 2562, 2561, 2553, 2552, 2552, 2549, 2550, 2550, 2550, 2548, 2552, 2546, 2542, 2536, 2539, 2533, 2535, 2533, 2534, 2532, 2534, 2536, 2530, 2528, 2524, 2529, 2522, 2528, 2527, 2538, 2542, 2542, 2544, 2541, 2548, 2549, 2545, 2548, 2549, 2547, 2547, 2542, 2544, 2540, 2543, 2543, 2545, 2547, 2550, 2553, 2556, 2556, 2553, 2552, 2543, 2546, 2541, 2545, 2544, 2546, 2546, 2552, 2550, 2547, 2547, 2547, 2548, 2549, 2553, 2548, 2551, 2545, 2547, 2544, 2547, 2547, 2545, 2544, 2547, 2547, 2546, 2546, 2541, 2546, 2544, 2546, 2540, 2549, 2547, 2548, 2547, 2548, 2547, 2549, 2549, 2545, 2549, 2546, 2546, 2546, 2547, 2543, 2552, 2544, 2550, 2548, 2550, 2555, 2553, 2554, 2551, 2557, 2552, 2557, 2550, 2559, 2556, 2559, 2556, 2556, 2553, 2551, 2550, 2549, 2546, 2548, 2547, 2543, 2546, 2543, 2551, 2549, 2554, 2552, 2557, 2554, 2550, 2555, 2552, 2557, 2559, 2561, 2561, 2563, 2557, 2560, 2554, 2559, 2554, 2556, 2552, 2552, 2550, 2549, 2551, 2554, 2555, 2552, 2554, 2552, 2553, 2548, 2549, 2553, 2551, 2551, 2554, 2555, 2557, 2557, 2556, 2558, 2555, 2560, 2553, 2557, 2554, 2555, 2556, 2556, 2555 },
		/* 5 hole */             { 2564, 2553, 2551, 2550, 2549, 2550, 2550, 2550, 2552, 2551, 2552, 2552, 2553, 2552, 2552, 2548, 2550, 2548, 2546, 2544, 2546, 2547, 2547, 2541, 2539, 2536, 2535, 2538, 2539, 2547, 2551, 2552, 2550, 2554, 2556, 2557, 2554, 2554, 2553, 2547, 2547, 2548, 2550, 2552, 2551, 2547, 2548, 2547, 2546, 2546, 2546, 2548, 2549, 2552, 2550, 2553, 2549, 2546, 2550, 2550, 2546, 2549, 2546, 2544, 2547, 2547, 2546, 2544, 2549, 2545, 2546, 2543, 2545, 2541, 2539, 2537, 2540, 2541, 2546, 2539, 2541, 2544, 2545, 2550, 2548, 2552, 2552, 2551, 2546, 2551, 2549, 2546, 2548, 2546, 2549, 2548, 2548, 2544, 2547, 2545, 2546, 2545, 2549, 2546, 2546, 2547, 2547, 2550, 2549, 2553, 2548, 2555, 2550, 2546, 2547, 2547, 2545, 2547, 2542, 2541, 2542, 2542, 2543, 2541, 2544, 2542, 2543, 2541, 2542, 2537, 2536, 2535, 2529, 2528, 2520, 2519, 2512, 2803, 2797, 2786, 2784, 2783, 3068, 3624, 3618, 3614, 3608, 3605, 3608, 3602, 3607, 3606, 3605, 3605, 3609, 3608, 3609, 3607, 3611, 3608, 3608, 3610, 3606, 3607, 3606, 3603, 3605, 3607, 3605, 3604, 3608, 3609, 3611, 3611, 3610, 3610, 3613, 3610, 3611, 3608, 3610, 3608, 3606, 3606, 3607, 3606, 3604, 3605, 3604, 3604, 3602, 3604, 3601, 3603, 3604, 3600, 3602, 3607, 3606, 3609, 3610, 3611, 3613, 3608, 3605, 3604, 3601, 3600, 3600, 3601, 3597, 3601, 3599, 3597, 3596, 3597, 3595, 3602, 3600, 3603, 3603, 3604, 3605, 3605, 3605, 3605, 3606, 3601, 3597, 3600, 3599, 3599, 3597, 3597, 3594, 3593, 3592, 3593, 3590, 3591, 3594, 3594, 3592, 3592, 3594, 3597, 3593, 3591, 3594, 3592, 3590, 3587, 3586, 3583, 3583, 3579, 3582, 3583, 3584, 3584, 3583, 3582, 3583, 3581, 3582, 3583, 3583, 3585, 3580, 3585, 3580, 3579, 3584, 3588, 3586, 3586, 3589, 3587, 3587, 3586, 3583, 3585, 3588, 3590, 3591, 3592, 3590, 3591, 3592, 3586, 3590, 3592, 3594, 3605, 3606, 3610, 3610, 3609, 3604, 3598, 3582, 3575, 3567, 3551, 3541, 3545, 3554, 3302, 2779, 2770, 2779, 2752, 2741, 2761, 2758, 2774, 2522, 2522, 2527, 2527, 2531, 2530, 2525, 2529, 2527, 2524, 2527, 2531, 2535, 2539, 2541, 2536, 2534, 2534, 2534, 2529, 2534, 2537, 2541, 2542, 2547, 2550, 2548, 2547, 2543, 2541, 2537, 2539, 2536, 2537, 2539, 2540, 2541, 2538, 2538, 2536, 2534, 2533, 2533, 2538, 2546, 2551, 2550, 2549, 2554, 2554, 2551, 2554, 2549, 2551, 2544, 2548, 2548, 2549, 2545, 2549, 2542, 2544, 2545, 2540, 2539, 2543, 2548, 2547, 2546, 2547, 2551, 2545, 2549, 2547, 2552, 2555, 2556, 2555, 2557, 2557, 2556, 2556, 2555, 2555, 2553, 2555, 2552, 2557, 2554, 2557, 2558, 2563, 2561, 2557, 2551, 2550, 2548, 2547, 2549, 2548, 2548, 2544, 2546, 2542, 2543, 2541, 2542, 2541, 2538, 2540, 2537, 2541, 2540, 2543, 2535, 2543, 2539, 2540, 2538, 2536, 2537, 2540, 2540, 2542, 2545, 2543, 2546, 2541, 2542, 2541, 2541, 2542, 2544, 2546 },
		/* 6 sunshine */         { 2553, 2548, 2555, 2557, 2556, 2550, 2540, 2534, 2527, 2530, 2526, 2529, 2535, 2536, 2539, 2538, 2535, 2535, 2533, 2533, 2531, 2531, 2531, 2531, 2533, 2531, 2530, 2532, 2535, 2543, 2555, 2576, 2600, 2625, 2637, 2645, 2649, 2653, 2656, 2658, 2659, 2660, 2661, 2664, 2662, 2663, 2659, 2662, 2664, 2662, 2663, 2664, 2664, 2666, 2664, 2663, 2661, 2660, 2658, 2661, 2659, 2659, 2656, 2658, 2661, 2660, 2662, 2664, 2665, 2663, 2666, 2664, 2663, 2658, 2650, 2635, 2620, 2602, 2580, 2564, 2551, 2545, 2539, 2538, 2538, 2537, 2530, 2529, 2526, 2527, 2528, 2523, 2523, 2526, 2525, 2529, 2530, 2527, 2526, 2524, 2525, 2525, 2528, 2524, 2529, 2525, 2529, 2529, 2527, 2524, 2526, 2526, 2531, 2531, 2531, 2529, 2529, 2529, 2530, 2532, 2535, 2538, 2544, 2552, 2567, 2584, 2605, 2620, 2643, 2652, 2659, 2659, 2658, 2659, 2657, 2657, 2661, 2660, 2659, 2660, 2657, 2658, 2654, 2651, 2641, 2627, 2616, 2603, 2586, 2569, 2558, 2550, 2549, 2546, 2543, 2542, 2539, 2536, 2535, 2534, 2532, 2530, 2529, 2530, 2529, 2532, 2531, 2530, 2528, 2529, 2529, 2527, 2527, 2530, 2529, 2530, 2532, 2531, 2532, 2531, 2534, 2533, 2532, 2531, 2530, 2532, 2530, 2531, 2532, 2532, 2530, 2532, 2530, 2531, 2533, 2530, 2526, 2527, 2529, 2529, 2532, 2529, 2530, 2531, 2527, 2525, 2527, 2528, 2530, 2532, 2534, 2529, 2527, 2529, 2530, 2528, 2530, 2529, 2530, 2531, 2530, 2529, 2529, 2533, 2534, 2534, 2534, 2531, 2533, 2533, 2531, 2532, 2533, 2531, 2535, 2536, 2538, 2534, 2530, 2529, 2528, 2530, 2528, 2529, 2531, 2528, 2531, 2529, 2530, 2530, 2529, 2528, 2528, 2527, 2529, 2528, 2529, 2529, 2532, 2530, 2532, 2531, 2531, 2529, 2527, 2529, 2529, 2530, 2526, 2528, 2525, 2528, 2530, 2529, 2529, 2528, 2527, 2527, 2530, 2530, 2530, 2532, 2533, 2530, 2532, 2531, 2531, 2530, 2530, 2532, 2529, 2529, 2530, 2529, 2531, 2529, 2527, 2533, 2530, 2534, 2534, 2529, 2532, 2531, 2531, 2533, 2532, 2531, 2534, 2535, 2533, 2534, 2533, 2530, 2529, 2527, 2529, 2528, 2532, 2527, 2526, 2529, 2528, 2530, 2531, 2529, 2532, 2534, 2537, 2548, 2551, 2558, 2570, 2584, 2597, 2607, 2616, 2630, 2640, 2647, 2650, 2652, 2656, 2658, 2661, 2661, 2659, 2658, 2656, 2655, 2653, 2653, 2652, 2651, 2637, 2622, 2603, 2583, 2568, 2551, 2548, 2545, 2541, 2542, 2540, 2539, 2539, 2536, 2534, 2534, 2534, 2530, 2533, 2529, 2528, 2528, 2531, 2532, 2534, 2532, 2531, 2532, 2528, 2530, 2524, 2523, 2528, 2529, 2528, 2529, 2529, 2525, 2524, 2526, 2525, 2526, 2530, 2533, 2531, 2532, 2537, 2541, 2548, 2561, 2572, 2591, 2602, 2616, 2625, 2636, 2643, 2647, 2650, 2650, 2655, 2656, 2657, 2659, 2660, 2659, 2657, 2657, 2656, 2659, 2658, 2655, 2656, 2657, 2656, 2659, 2660, 2661, 2661, 2665, 2664, 2663, 2661, 2659, 2661, 2662, 2663, 2666, 2662, 2662, 2660, 2656, 2652, 2648, 2645, 2634 },
		/* 7 black_beauty */     { 2536, 2527, 2525, 2527, 2530, 2530, 2532, 2535, 2530, 2529, 2526, 2531, 2528, 2528, 2528, 2528, 2527, 2530, 2528, 2530, 2528, 2528, 2527, 2527, 2526, 2530, 2523, 2525, 2526, 2530, 2530, 2530, 2528, 2527, 2531, 2530, 2533, 2533, 2532, 2530, 2531, 2533, 2535, 2536, 2539, 2545, 2555, 2566, 2588, 2606, 2621, 2633, 2642, 2648, 2650, 2652, 2652, 2653, 2654, 2654, 2653, 2651, 2652, 2651, 2650, 2651, 2646, 2642, 2628, 2609, 2594, 2578, 2562, 2553, 2546, 2541, 2541, 2538, 2536, 2534, 2534, 2532, 2535, 2532, 2532, 2530, 2531, 2529, 2529, 2529, 2530, 2531, 2530, 2531, 2527, 2528, 2527, 2530, 2528, 2531, 2531, 2532, 2528, 2529, 2530, 2533, 2528, 2527, 2525, 2529, 2527, 2531, 2528, 2533, 2535, 2546, 2554, 2570, 2591, 2616, 2632, 2644, 2650, 2658, 2659, 2662, 2664, 2664, 2661, 2663, 2662, 2659, 2661, 2657, 2653, 2648, 2644, 2634, 2622, 2609, 2591, 2573, 2561, 2555, 2549, 2544, 2544, 2540, 2534, 2533, 2532, 2528, 2527, 2529, 2528, 2531, 2530, 2530, 2529, 2526, 2524, 2527, 2528, 2526, 2527, 2530, 2530, 2532, 2534, 2535, 2534, 2530, 2529, 2530, 2531, 2530, 2531, 2530, 2533, 2535, 2532, 2535, 2529, 2528, 2528, 2528, 2527, 2529, 2530, 2530, 2529, 2528, 2530, 2531, 2532, 2529, 2533, 2529, 2527, 2527, 2528, 2530, 2531, 2533, 2534, 2532, 2533, 2532, 2530, 2529, 2529, 2529, 2531, 2530, 2531, 2531, 2532, 2529, 2530, 2532, 2531, 2532, 2532, 2528, 2531, 2530, 2536, 2533, 2535, 2531, 2533, 2534, 2533, 2531, 2532, 2532, 2531, 2530, 2530, 2531, 2533, 2531, 2531, 2533, 2534, 2531, 2531, 2528, 2528, 2529, 2526, 2528, 2531, 2531, 2534, 2536, 2535, 2534, 2533, 2531, 2528, 2530, 2529, 2528, 2531, 2530, 2533, 2532, 2533, 2533, 2536, 2534, 2531, 2532, 2532, 2531, 2533, 2535, 2532, 2532, 2532, 2532, 2533, 2534, 2534, 2536, 2535, 2534, 2533, 2534, 2533, 2532, 2532, 2533, 2531, 2533, 2535, 2532, 2531, 2531, 2531, 2527, 2531, 2532, 2535, 2536, 2539, 2540, 2535, 2532, 2533, 2535, 2531, 2532, 2530, 2530, 2529, 2535, 2534, 2541, 2546, 2550, 2557, 2565, 2567, 2580, 2590, 2596, 2609, 2623, 2639, 2648, 2655, 2657, 2658, 2657, 2660, 2662, 2663, 2662, 2663, 2665, 2662, 2651, 2638, 2630, 2621, 2608, 2597, 2588, 2576, 2564, 2551, 2547, 2544, 2543, 2540, 2542, 2540, 2539, 2536, 2535, 2535, 2533, 2533, 2534, 2536, 2536, 2534, 2531, 2533, 2533, 2532, 2534, 2532, 2533, 2533, 2535, 2532, 2534, 2536, 2534, 2532, 2534, 2533, 2534, 2534, 2536, 2538, 2542, 2545, 2555, 2563, 2568, 2576, 2584, 2594, 2601, 2609, 2621, 2637, 2645, 2651, 2653, 2656, 2656, 2659, 2658, 2660, 2661, 2661, 2660, 2654, 2648, 2636, 2626, 2612, 2607, 2598, 2588, 2575, 2566, 2553, 2546, 2537, 2537, 2538, 2540, 2538, 2534, 2536, 2533, 2535, 2536, 2538, 2538, 2538, 2537, 2536, 2532, 2533, 2530, 2531, 2531, 2531, 2532, 2535, 2533, 2533, 2533 },
		/* 8 white_innoccence */ { 2956, 2905, 2862, 2793, 2759, 2681, 2652, 2603, 2576, 2567, 2556, 2550, 2549, 2536, 2535, 2531, 2533, 2529, 2527, 2529, 2531, 2532, 2529, 2531, 2530, 2528, 2529, 2527, 2530, 2539, 2549, 2559, 2568, 2579, 2595, 2607, 2618, 2624, 2638, 2647, 2653, 2658, 2656, 2660, 2663, 2662, 2659, 2658, 2658, 2662, 2661, 2660, 2659, 2664, 2664, 2663, 2662, 2665, 2663, 2663, 2662, 2655, 2657, 2659, 2659, 2659, 2656, 2658, 2658, 2659, 2656, 2655, 2656, 2652, 2650, 2641, 2636, 2627, 2618, 2608, 2601, 2589, 2581, 2575, 2563, 2554, 2550, 2546, 2541, 2538, 2535, 2537, 2533, 2537, 2536, 2536, 2532, 2533, 2537, 2537, 2538, 2546, 2557, 2567, 2580, 2592, 2604, 2615, 2627, 2635, 2647, 2655, 2657, 2662, 2660, 2659, 2661, 2666, 2664, 2664, 2669, 2667, 2667, 2664, 2664, 2664, 2667, 2662, 2665, 2667, 2665, 2663, 2662, 2662, 2661, 2662, 2661, 2661, 2663, 2659, 2660, 2658, 2661, 2659, 2656, 2651, 2650, 2645, 2634, 2629, 2615, 2609, 2602, 2594, 2584, 2574, 2567, 2560, 2552, 2550, 2547, 2543, 2541, 2539, 2538, 2537, 2532, 2536, 2538, 2537, 2537, 2539, 2538, 2539, 2536, 2535, 2539, 2541, 2537, 2535, 2536, 2536, 2536, 2537, 2534, 2533, 2534, 2532, 2533, 2536, 2537, 2535, 2539, 2537, 2538, 2533, 2535, 2539, 2538, 2536, 2536, 2536, 2539, 2536, 2531, 2532, 2532, 2532, 2534, 2535, 2537, 2537, 2538, 2537, 2537, 2537, 2535, 2539, 2541, 2537, 2539, 2536, 2538, 2537, 2537, 2539, 2538, 2538, 2541, 2538, 2540, 2535, 2531, 2534, 2534, 2534, 2532, 2534, 2533, 2532, 2533, 2531, 2534, 2537, 2538, 2534, 2540, 2544, 2543, 2541, 2541, 2540, 2541, 2537, 2534, 2536, 2538, 2538, 2537, 2537, 2538, 2538, 2542, 2538, 2536, 2535, 2537, 2540, 2538, 2537, 2534, 2537, 2539, 2537, 2535, 2533, 2535, 2536, 2536, 2535, 2533, 2533, 2533, 2533, 2534, 2536, 2537, 2538, 2537, 2536, 2536, 2539, 2538, 2542, 2536, 2532, 2536, 2537, 2537, 2539, 2537, 2543, 2540, 2539, 2538, 2540, 2539, 2540, 2540, 2541, 2542, 2543, 2541, 2543, 2539, 2538, 2537, 2534, 2538, 2540, 2540, 2540, 2538, 2539, 2537, 2536, 2535, 2538, 2533, 2533, 2537, 2542, 2544, 2543, 2545, 2550, 2553, 2560, 2569, 2584, 2602, 2613, 2622, 2634, 2646, 2652, 2654, 2657, 2662, 2671, 2669, 2665, 2664, 2668, 2673, 2669, 2666, 2665, 2671, 2669, 2666, 2666, 2670, 2674, 2676, 2678, 2668, 2670, 2673, 2675, 2672, 2669, 2668, 2671, 2674, 2671, 2671, 2670, 2670, 2672, 2669, 2666, 2666, 2672, 2663, 2657, 2641, 2624, 2601, 2577, 2557, 2550, 2549, 2543, 2542, 2542, 2540, 2541, 2541, 2541, 2540, 2544, 2542, 2538, 2540, 2543, 2548, 2553, 2555, 2560, 2574, 2589, 2604, 2617, 2633, 2645, 2653, 2661, 2663, 2670, 2669, 2668, 2669, 2668, 2670, 2668, 2671, 2671, 2671, 2670, 2674, 2670, 2671, 2672, 2672, 2676, 2674, 2674, 2674, 2673, 2675, 2672, 2672, 2670, 2672, 2671, 2674, 2673, 2678, 2677 },
		/* 9 deep_blue */        { 2559, 2554, 2552, 2545, 2544, 2540, 2537, 2536, 2535, 2532, 2535, 2534, 2531, 2529, 2531, 2534, 2533, 2534, 2532, 2532, 2532, 2530, 2530, 2528, 2535, 2533, 2533, 2534, 2535, 2532, 2534, 2535, 2534, 2534, 2533, 2533, 2533, 2531, 2534, 2528, 2529, 2530, 2532, 2529, 2533, 2539, 2551, 2568, 2593, 2620, 2640, 2645, 2649, 2651, 2653, 2653, 2657, 2654, 2656, 2652, 2651, 2651, 2652, 2651, 2647, 2644, 2641, 2634, 2620, 2607, 2591, 2579, 2566, 2560, 2551, 2548, 2544, 2544, 2543, 2539, 2534, 2534, 2531, 2533, 2531, 2534, 2532, 2533, 2532, 2535, 2533, 2532, 2532, 2534, 2531, 2536, 2528, 2531, 2530, 2531, 2528, 2529, 2530, 2531, 2543, 2557, 2580, 2611, 2635, 2641, 2648, 2649, 2656, 2656, 2658, 2656, 2658, 2657, 2658, 2657, 2656, 2658, 2656, 2655, 2659, 2658, 2657, 2656, 2656, 2654, 2655, 2656, 2657, 2655, 2652, 2652, 2656, 2655, 2652, 2651, 2651, 2649, 2653, 2651, 2649, 2647, 2645, 2642, 2639, 2632, 2624, 2611, 2596, 2585, 2574, 2567, 2560, 2553, 2548, 2541, 2541, 2538, 2537, 2533, 2532, 2529, 2529, 2526, 2526, 2523, 2522, 2519, 2517, 2519, 2521, 2521, 2530, 2557, 2580, 2607, 2635, 2649, 2657, 2658, 2661, 2660, 2662, 2657, 2657, 2660, 2660, 2661, 2661, 2663, 2665, 2663, 2664, 2661, 2659, 2659, 2656, 2657, 2652, 2651, 2649, 2650, 2646, 2647, 2642, 2639, 2631, 2625, 2616, 2610, 2604, 2598, 2592, 2588, 2588, 2586, 2586, 2585, 2585, 2584, 2583, 2582, 2583, 2581, 2582, 2582, 2577, 2576, 2575, 2573, 2574, 2574, 2578, 2580, 2580, 2582, 2582, 2582, 2583, 2588, 2589, 2593, 2596, 2600, 2606, 2610, 2612, 2618, 2625, 2630, 2634, 2639, 2645, 2645, 2650, 2650, 2654, 2654, 2657, 2657, 2657, 2656, 2659, 2654, 2654, 2657, 2657, 2659, 2659, 2661, 2663, 2667, 2666, 2670, 2667, 2669, 2658, 2638, 2629, 2632, 2614, 2597, 2571, 2543, 2527, 2524, 2524, 2524, 2529, 2530, 2532, 2536, 2536, 2535, 2535, 2534, 2536, 2536, 2539, 2540, 2546, 2549, 2558, 2563, 2572, 2578, 2588, 2595, 2605, 2618, 2630, 2635, 2643, 2644, 2648, 2649, 2652, 2651, 2651, 2651, 2652, 2654, 2655, 2655, 2654, 2655, 2652, 2658, 2657, 2658, 2656, 2659, 2656, 2657, 2655, 2655, 2654, 2657, 2659, 2661, 2663, 2661, 2659, 2663, 2661, 2663, 2662, 2656, 2643, 2637, 2633, 2625, 2615, 2586, 2558, 2540, 2536, 2533, 2534, 2535, 2538, 2535, 2535, 2534, 2534, 2534, 2533, 2532, 2535, 2531, 2534, 2532, 2531, 2531, 2530, 2530, 2532, 2533, 2532, 2538, 2540, 2543, 2547, 2556, 2559, 2563, 2570, 2580, 2591, 2606, 2620, 2633, 2641, 2648, 2653, 2654, 2655, 2656, 2661, 2656, 2661, 2658, 2659, 2656, 2654, 2645, 2637, 2625, 2618, 2610, 2597, 2579, 2557, 2544, 2536, 2532, 2534, 2536, 2536, 2542, 2540, 2540, 2536, 2537, 2534, 2534, 2532, 2534, 2531, 2536, 2537, 2538, 2537, 2541, 2539, 2539, 2540, 2537, 2537, 2538, 2538, 2538, 2539, 2538, 2540, 2538 }
};

Height_Measurement::Height_Measurement(){
			last_type = 0;
			is_stopped = false;
			height_diff = 0;
			timer_cid = 0;
			timer_con = 0;
};

void Height_Measurement::execute(void*){
    /* Zugriffsrechte von QNX fuer diesen Thread, auf die Hardware erbitten. */
    if( ThreadCtl(_NTO_TCTL_IO_PRIV,0) == -1 ){
        cout << "Can't get Hardware access, therefore can't do anything." << endl;
    }

	uint32_t msg;
	uint32_t level;
	struct height_array ha;
	Height_sensor* height_sensor = Height_sensor::get_instance();
	get_height_difference(height_sensor);
	while(1){
		level = height_sensor -> get_height();
		if(level < (GROUND_HEIGHT + height_diff)){
			ha = get_height_array();
			
			// !!!!!!--------!!!!!!!!!!
			// for profile measurement
//				cout << "[ ";
//				for(msg = 0; msg < ha.length; msg++){
//					cout << ha.array[msg] << ", ";
//				}
//				cout << "]" << endl;
			// !!!!!!--------!!!!!!!!!!
			
			if(ha.length >= MIN_LENGTH){
				last_type = compare_arrays(ha);
				msg = (last_type == UNDEFINED ? INVALID_PUK_TYPE_ID_E : HM_DONE_EVENT_ID_E);
				MsgSendPulse(3, -1, 5, msg);
			}
		} 
	}
}

void Height_Measurement::shutdown(){
	// ignore
}

void  Height_Measurement::set_motor_speed(uint32_t speed){
	is_stopped = (speed == STOPPED);
}

uint32_t Height_Measurement::get_type(void){
	return last_type;
}

height_array Height_Measurement::get_height_array(void){
	Height_sensor* height_sensor = Height_sensor::get_instance();
	struct _pulse pulse;
	std::vector<uint32_t> height_vector;
	bool in_measurement = true;
	uint32_t length_counter = 0;
	uint32_t level;
	uint32_t ground_debouncer = 0;
	struct height_array ha;
	do{
		// wait for tick
		MsgReceivePulse(timer_cid, &pulse, sizeof (pulse), NULL);

		level = height_sensor -> get_height();
		if(++length_counter >= MAX_LENGTH || level <= MAX_HEIGHT){
			ha.length = 1;
			height_vector.clear();
			height_vector.push_back(INVALID_PUK_TYPE_ID_E);
			ha.array = &height_vector[0];
			MsgSendPulse(3, -1, 5, INVALID_PUK_TYPE_ID_E);
			return ha;
		}else if(level >= GROUND_HEIGHT){
			if(ground_debouncer >= 4){
				in_measurement = false;
			}
			++ground_debouncer;
		}else{
			height_vector.push_back(level);
		}
		if(length_counter == MIN_LENGTH ){
			MsgSendPulse(3, -1, 5, HM_START_EVENT_ID_E);
		}
		while(is_stopped);
	}while(in_measurement);
	if(!height_vector.empty()){
		ha.length = height_vector.size();
		ha.array = &height_vector[0];
		return ha;
	}
	ha.length = 1;
	height_vector.clear();
	height_vector.push_back(UNDEFINED);
	ha.array = &height_vector[0];
	return ha;
}

uint32_t Height_Measurement::compare_arrays(height_array ha){
	uint32_t sum_array[SUM_PUKS];
	uint32_t i;
	uint32_t j;
	int32_t temp;

	if(ha.length < PROTOTYPE_LENGTH + CUT_OFF_FRONT){
		return UNDEFINED;
	}
	for(j = 0; j < SUM_PUKS; j++){
		sum_array[j] = 0;
		for(i = 0; i < PROTOTYPE_LENGTH; i++){
			temp = (int32_t) ha.array[i + CUT_OFF_FRONT] - (int32_t) profiles[j][i] + height_diff;
			if(temp < 0){
				temp *= -1;
			}
			sum_array[j] += temp;
		}
	}

	j = 0;
	for(i = 1; i < SUM_PUKS; i++){
		if(sum_array[i] < sum_array[j]){
			j = i;
		}
	}

	if(sum_array[j] <= SMALLEST_ABS_ERROR){
		j = (j < SUM_IC_WM ? 0 : j);
		return j;
	}
	return UNDEFINED;
}

void Height_Measurement::get_height_difference(Height_sensor* height_sensor){
	int i;
	height_diff = 0;
	for(i = 0; i < INIT_MEASUREMENTS; i++){
		height_diff += height_sensor -> get_height();
	}
	height_diff = height_diff / INIT_MEASUREMENTS - INIT_HEIGHT;
	cout << "height_diff" << height_diff << endl;
}

void  Height_Measurement::init_timer(){
	// init pulse message
	if( (timer_cid  = ChannelCreate(0)) == -1){
		cout << "ChannelCreate() failed"<< endl;
	}
	if( (timer_con = ConnectAttach(0, 0, timer_cid, 0, 0)) == -1){
		cout << "ConnectAttach() failed"<< endl;
	}
	SIGEV_PULSE_INIT (&timerEvent, timer_con, SIGEV_PULSE_PRIO_INHERIT, 0, 0);
    if (timer_create (CLOCK_REALTIME, &timerEvent, &timerId) == -1) {
    	cout << "timer_create() failed"<< endl;
    }

    // init timer
	timerSpec.it_value.tv_sec = TICK_SEC;
	timerSpec.it_value.tv_nsec = TICK_NSEC;
	timerSpec.it_interval.tv_sec = TICK_SEC;
	timerSpec.it_interval.tv_nsec = TICK_NSEC;
	timer_settime (timerId, 0, &timerSpec, NULL);
}

