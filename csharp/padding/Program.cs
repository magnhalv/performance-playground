using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct Entry
{
    public int value;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 255)]
    public int[] padding;
}

class Program
{
    static void FillArray(Entry[] entries)
    {
        var random = new Random();
        for (int i = 0; i < entries.Length; i++)
        {
            entries[i].value = random.Next(1, 11);
            entries[i].padding = new int[255];
        }
    }

    static TimeSpan AddTogether(int size, int numTimes)
    {
        var duration = TimeSpan.Zero;
        Entry[] arr1 = new Entry[size];
        Entry[] arr2 = new Entry[size];

        FillArray(arr1);
        FillArray(arr2);

        for (int num = 0; num < numTimes + 1; num++)
        {
            int result = 0;
            var stopwatch = Stopwatch.StartNew();
            for (int i = 0; i < size; i++)
            {
                result += (arr1[i].value + arr2[i].value);
            }
            stopwatch.Stop();
            if (num != 0)
            {
                duration += stopwatch.Elapsed;
            }
        }
        return TimeSpan.FromTicks(duration.Ticks / numTimes);
    }

    static void Main(string[] args)
    {
        Console.WriteLine($"Running with entry size: {Marshal.SizeOf<Entry>()}");

        for (int n = 1; n < 100; n++)
        {
            int N = 30700 * n;
            var duration = AddTogether(N, 30);
            Console.WriteLine($"  N = {N}: {duration.TotalMilliseconds * 1000} microseconds.");
        }
    }
}
