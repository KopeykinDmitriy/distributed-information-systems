namespace DeadLock
{
    class Program
    {
        private static object _lockOne = new();
        private static object _lockTwo = new();
        private static int _counterOne = 0;
        private static int _counterTwo = 0;
        
        static void Main(string[] args)
        {
            var threadOne = new Thread(IncreaseCountersFirst);
            var threadTwo = new Thread(IncreaseCountersSecond);
            
            threadOne.Start();
            threadTwo.Start();

            while (true)
            {
                Console.WriteLine($"{_counterOne}    {_counterTwo}");
                Console.WriteLine($"{threadOne.ThreadState}");
            }
        }

        private static void IncreaseCountersFirst()
        {
            for (var i = 0; i < 10000; i++)
            {
                lock (_lockOne)
                {
                    _counterOne++;
                    lock (_lockTwo)
                    {
                        _counterTwo++;
                    }
                }
            }
        }

        private static void IncreaseCountersSecond()
        {
            for (var i = 0; i < 10000; i++)
            {
                lock (_lockTwo)
                {
                    _counterTwo++;
                    lock (_lockOne)
                    {
                        _counterOne++;
                    }
                }
            }
        }
    }
}