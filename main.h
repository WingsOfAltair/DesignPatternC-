#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Abstract Command Interface
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() {}
};

// Receiver class
class Calculator {
private:
    float result = 0.0f;

public:
    void add(float value) {
        result += value;
    }

    void subtract(float value) {
        result -= value;
    }

    void multiply(float value) {
        result *= value;
    }

    void divide(float value) {
        if (value != 0)
            result /= value;
        else
            std::cerr << "Error: Division by zero" << std::endl;
    }

    void clear() {
        result = 0;
    }

    float getResult() const {
        return result;
    }
};

// Concrete Command for Addition
class AddCommand : public Command {
private:
    Calculator& calculator;
    float value;

public:
    AddCommand(Calculator& calc, float val) : calculator(calc), value(val) {}

    void execute() override {
        calculator.add(value);
    }

    void undo() override {
        calculator.subtract(value);
    }
};

// Concrete Command for Subtraction
class SubtractCommand : public Command {
private:
    Calculator& calculator;
    float value;

public:
    SubtractCommand(Calculator& calc, float val) : calculator(calc), value(val) {}

    void execute() override {
        calculator.subtract(value);
    }

    void undo() override {
        calculator.add(value);
    }
};

// Concrete Command for Multiplication
class MultiplyCommand : public Command {
private:
    Calculator& calculator;
    float value;
    float previousValue;

public:
    MultiplyCommand(Calculator& calc, float val) : calculator(calc), value(val) {}

    void execute() override {
        previousValue = calculator.getResult();
        calculator.multiply(value);
    }

    void undo() override {
        calculator.divide(value);
        calculator.add(previousValue);
    }
};

// Concrete Command for Division
class DivideCommand : public Command {
private:
    Calculator& calculator;
    float value;
    float previousValue;

public:
    DivideCommand(Calculator& calc, float val) : calculator(calc), value(val) {}

    void execute() override {
        previousValue = calculator.getResult();
        calculator.divide(value);
    }

    void undo() override {
        calculator.multiply(value);
        calculator.add(previousValue);
    }
};

// Concrete Command for Clearing
class ClearCommand : public Command {
private:
    Calculator& calculator;
    float previousValue;

public:
    ClearCommand(Calculator& calc) : calculator(calc) {}

    void execute() override {
        previousValue = calculator.getResult();
        calculator.clear();
    }

    void undo() override {
        calculator.add(previousValue);
    }
};

// Invoker class
class CalculatorControl {
private:
    std::vector<std::unique_ptr<Command>> commands;

public:
    void executeCommand(std::unique_ptr<Command> command) {
        command->execute();
        commands.push_back(std::move(command));
    }

    void undoLastCommand() {
        if (!commands.empty()) {
            auto lastCommand = std::move(commands.back());
            lastCommand->undo();
            commands.pop_back();
        }
    }
};

int main() {
    Calculator calculator;
    CalculatorControl calculatorControl;

    std::string operation;
    double value;

    while (true) {
        std::cout << "Enter operation (add, subtract, multiply, divide, clear, undo, exit): ";
        std::cin >> operation;

        if (operation == "exit")
            break;

        if (operation != "undo" && operation != "clear") {
            std::cout << "Enter value: ";
            std::cin >> value;
        }

        if (operation == "add") {
            calculatorControl.executeCommand(std::make_unique<AddCommand>(calculator, value));
        }
        else if (operation == "subtract") {
            calculatorControl.executeCommand(std::make_unique<SubtractCommand>(calculator, value));
        }
        else if (operation == "multiply") {
            calculatorControl.executeCommand(std::make_unique<MultiplyCommand>(calculator, value));
        }
        else if (operation == "divide") {
            calculatorControl.executeCommand(std::make_unique<DivideCommand>(calculator, value));
        }
        else if (operation == "clear") {
            calculatorControl.executeCommand(std::make_unique<ClearCommand>(calculator));
        }
        else if (operation == "undo") {
            calculatorControl.undoLastCommand();
        }
        else {
            std::cerr << "Invalid operation. Please try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "Result: " << calculator.getResult() << std::endl;
    }

    std::cout << "Final Result: " << calculator.getResult() << std::endl;

    return 0;
}
