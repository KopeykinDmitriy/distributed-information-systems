namespace RaceCondition
{
	class Program
	{
		private static int _counter = 0;
		static void Main(string[] args)
		{
			var threadOne = new Thread(IncreaseCounter);
			var threadTwo = new Thread(IncreaseCounter);
			
			threadOne.Start();
			threadTwo.Start();
			
			Thread.Sleep(3000);
			Console.WriteLine($"Ожидаемый результат counter = 20000\nФактический результат counter = {_counter}");
		}

		private static void IncreaseCounter()
		{
			for (var i = 0; i < 10000; i++)
				_counter++;
		}
	}
}