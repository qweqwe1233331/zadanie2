using Castle.Facilities.Startable;
using Castle.MicroKernel.Registration;
using Castle.MicroKernel.Resolvers.SpecializedResolvers;
using Castle.MicroKernel.SubSystems.Configuration;
using Castle.Windsor;
using System;
using System.Collections.Generic;
using System.Linq;

namespace CalculatorCsharp

    public interface IOperationProvider
{
    public IEnumerable<Operation> Get();
}

public class OperationProvider : IOperationProvider
{
    private IEnumerable<Operation> operations;

    public OperationProvider(IEnumerable<Operation> operations)
    {
        this.operations = operations;
    }

    public IEnumerable<Operation> Get()
    {
        return operations;
    }
}


public class Application
{
    public Application(
        IOperationProvider operationProvider,
        IMenu menu)
    {
        this.operationProvider = operationProvider;
        this.menu = menu;
    }

    private IOperationProvider operationProvider;
    private IMenu menu;
    private IEnumerable<Operation> operations;

    public void Run()
    {
        operations = operationProvider.Get();
        Operation operation = menu.Show(operations.ToArray());

        Console.Write("Введите первое число: ");
        double num1 = Convert.ToDouble(Console.ReadLine());
        Console.Write("Введите второе число: ");
        double num2 = Convert.ToDouble(Console.ReadLine());

        double result = operation.Run(num1, num2);
        Console.WriteLine("Результат: " + result);
    }
}

public class Program
{
    private static IWindsorContainer _container = new WindsorContainer();

    static void Main(string[] args)
    {
        try {
            Start();
        }
        catch (Exception e) {
            Console.WriteLine(e.ToString());
        }
        finally {
            _container ? .Dispose();
        }
    }

    private static void Start()
    {
        _container.AddFacility<StartableFacility>(f = > f.DeferredStart());
        _container.Kernel.Resolver.AddSubResolver(new CollectionResolver(_container.Kernel));
        _container.Install(new LocalInstaller());
    }
}

internal class LocalInstaller : IWindsorInstaller
{
    public void Install(IWindsorContainer container, IConfigurationStore store)
    {
        container.Register(
            Component.For<IWindsorContainer>().Instance(container),
            Component.For<Application>()
            .StartUsingMethod("Run"),

            Component.For<IMenu>()
            .ImplementedBy<NewMenu>()
            .LifestyleTransient(),

            Component.For<IMenu>()
            .ImplementedBy<Menu>()
            .LifestyleTransient(),

            Component.For<IOperationProvider>()
            .ImplementedBy<OperationProvider>(),

            Component.For<Operation>()
            .ImplementedBy<Addition>(),
            Component.For<Operation>()
            .ImplementedBy<Substraction>(),
            Component.For<Operation>()
            .ImplementedBy<Multiplacation>(),
            Component.For<Operation>()
            .ImplementedBy<Division>(),
            Component.For<Operation>()
            .ImplementedBy<Sqrt>(),
            Component.For<Operation>()
            .ImplementedBy<Cos>(),
            Component.For<Operation>()
            .ImplementedBy<Sin>(),
            Component.For<Operation>()
            .ImplementedBy<Tg>()
        );
    }
}

public interface IMenu
{
    public Operation Show(Operation[] operations);
}

public sealed class Menu : IMenu
{
    public Operation Show(Operation[] operations)
    {
        Console.WriteLine("======== КАЛЬКУЛЯТОР ==========");
        for (int i = 0; i < operations.Length; i++)
        {
            Operation operation = operations[i];
            Console.WriteLine($"{i + 1}. ОПЕРАЦИЯ {operation.Name};");
        }
        Console.Write("Выберите действие: ");
        return operations[Convert.ToInt32(Console.ReadLine()) - 1];
    }
}

public sealed class NewMenu : IMenu
{
    public Operation Show(Operation[] operations)
    {
        Console.WriteLine("....КАЛЬКУЛЯТОР....");
        for (int i = 0; i < operations.Length; i++)
        {
            Operation operation = operations[i];
            Console.WriteLine($"{i + 1}. {operation.Name};");
        }
        Console.Write("Выберите действие: ");
        return operations[Convert.ToInt32(Console.ReadLine()) - 1];
    }
}

public abstract class Operation
{
    public abstract string Name{ get; }

    public abstract double Run(params double[] numbers);
}

public sealed class Addition : Operation
{
    public override string Name = > "Сложение";

    public override double Run(params double[] numbers)
    {
        double result = numbers.Sum();
        return result;
    }
}

public sealed class Substraction : Operation
{
    public override string Name = > "Вычитание";

    public override double Run(params double[] numbers)
    {
        double result = numbers[0];
        for (int i = 0; i < numbers.Length; i++)
        {
            result -= numbers[i];
        }
        return result;
    }
}

public sealed class Multiplacation : Operation
{
    public override string Name = > "Умножение";

    public override double Run(params double[] numbers)
    {
        double result = numbers[0];
        for (int i = 0; i < numbers.Length; i++)
        {
            result *= numbers[i];
        }
        return result;
    }
}

public sealed class Division : Operation
{
    public override string Name = > "Деление";

    public override double Run(params double[] numbers)
    {
        double result = numbers[0];
        for (int i = 0; i < numbers.Length; i++)
        {
            result /= numbers[i];
        }
        return result;
    }
}

public sealed class Sqrt : Operation
{
    public override string Name = > "Квадратный корень";

    public override double Run(params double[] numbers)
    {
        return 0d;
    }
}

public sealed class Cos : Operation
{
    public override string Name = > "Косинус";

    public override double Run(params double[] numbers)
    {
        return 0d;
    }
}

public sealed class Sin : Operation
{
    public override string Name = > "Синус";

    public override double Run(params double[] numbers)
    {
        return 0d;
    }
}

public sealed class Tg : Operation
{
    public override string Name = > "Тангенс";

    public override double Run(params double[] numbers)
    {
        return 0d;
    }
}