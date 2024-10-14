using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

struct BadAlignment
{
    public int value;
    public bool a;
    public long b;
    public bool c;
    public long d;
    public bool e;
    public long f;
    public bool g;
    public long h;
}

struct MidAlignment
{
    public bool a;
    public long b;
    public bool c;
    public long d;
    public int value;
    public bool e;
    public long f;
    public bool g;
    public long h;
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct GoodAlignment
{
    public int value;
    public long b;
    public long d;
    public long f;
    public long h;
    public bool a;
    public bool c;
    public bool e;
    public bool g;
}

class Program
{
    static void FillArray(BadAlignment[] entries)
    {
        var random = new Random();
        for (int i = 0; i < entries.Length; i++)
        {
            entries[i].value = random.Next(1, 11);
        }
    }

    static void Main(string[] args)
    {
        Console.WriteLine($"BadAlignment size: {Marshal.SizeOf<BadAlignment>()}");

        const int NUM_TIMES = 30;
        const int NUM_ITERATIONS = 1;
        int N = 3070000;

        for (int n = 0; n < NUM_ITERATIONS; n++)
        {
            BadAlignment[] bad1 = new BadAlignment[N];
            BadAlignment[] bad2 = new BadAlignment[N];

            for (int i = 0; i < N; i++)
            {
                bad1[i].value = 1;
                bad2[i].value = 2;
            }

            var duration = TimeSpan.Zero;
            for (int num = 0; num < NUM_TIMES + 1; num++)
            {
                int result = 0;

                var stopwatch = Stopwatch.StartNew();
                for (int i = 0; i < N; i++)
                {
                    result += (bad1[i].value + bad2[i].value);
                }
                stopwatch.Stop();

                if (num != 0)
                {
                    duration += stopwatch.Elapsed;
                }
            }
            Console.WriteLine($"  N = {N}: {duration.TotalMilliseconds * 1000} microseconds.");
        }

        Console.WriteLine($"GoodAlignment size: {Marshal.SizeOf<GoodAlignment>()}");
        for (int n = 0; n < NUM_ITERATIONS; n++)
        {
            GoodAlignment[] good1 = new GoodAlignment[N];
            GoodAlignment[] good2 = new GoodAlignment[N];

            for (int i = 0; i < N; i++)
            {
                good1[i].value = 1;
                good2[i].value = 2;
            }

            var duration = TimeSpan.Zero;
            for (int num = 0; num < NUM_TIMES + 1; num++)
            {
                int result = 0;

                var stopwatch = Stopwatch.StartNew();
                for (int i = 0; i < N; i++)
                {
                    result += (good1[i].value + good2[i].value);
                }
                stopwatch.Stop();

                if (num != 0)
                {
                    duration += stopwatch.Elapsed;
                }
            }
            Console.WriteLine($"  N = {N}: {duration.TotalMilliseconds * 1000} microseconds.");
        }

        Console.WriteLine($"MidAlignment size: {Marshal.SizeOf<MidAlignment>()}");
        for (int n = 0; n < NUM_ITERATIONS; n++)
        {
            MidAlignment[] mid1 = new MidAlignment[N];
            MidAlignment[] mid2 = new MidAlignment[N];

            for (int i = 0; i < N; i++)
            {
                mid1[i].value = 1;
                mid2[i].value = 2;
            }

            var duration = TimeSpan.Zero;
            for (int num = 0; num < NUM_TIMES + 1; num++)
            {
                int result = 0;

                var stopwatch = Stopwatch.StartNew();
                for (int i = 0; i < N; i++)
                {
                    result += (mid1[i].value + mid2[i].value);
                }
                stopwatch.Stop();

                if (num != 0)
                {
                    duration += stopwatch.Elapsed;
                }
            }
            Console.WriteLine($"  N = {N}: {duration.TotalMilliseconds * 1000} microseconds.");
        }
    }
}
