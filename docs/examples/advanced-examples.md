# Advanced Examples

This page demonstrates sophisticated usage patterns of the Modern C++ Calculator library. These examples showcase complex mathematical systems, optimization techniques, and real-world applications that go beyond basic arithmetic.

## Scientific Computing

### Numerical Methods Implementation

Implement numerical algorithms using the calculator for dynamic formula evaluation:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <cmath>

class NumericalMethods {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    NumericalMethods() {
        setup_math_functions();
    }
    
    void setup_math_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("exp", [](double x) { return std::exp(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
    }
    
    // Newton-Raphson method for root finding
    double find_root(const std::string& function, const std::string& derivative,
                     double initial_guess, double tolerance = 1e-10, int max_iterations = 100) {
        double x = initial_guess;
        
        for (int i = 0; i < max_iterations; ++i) {
            parser_.set("x", x);
            double f_x = parser_.eval(function);
            double df_x = parser_.eval(derivative);
            
            if (std::abs(f_x) < tolerance) {
                std::cout << "Converged after " << i << " iterations" << std::endl;
                return x;
            }
            
            if (std::abs(df_x) < tolerance) {
                throw std::runtime_error("Derivative too small, cannot continue");
            }
            
            x = x - f_x / df_x;
        }
        
        throw std::runtime_error("Failed to converge within maximum iterations");
    }
    
    // Numerical integration using adaptive Simpson's rule
    double integrate(const std::string& function, double a, double b, double tolerance = 1e-8) {
        return adaptive_simpson(function, a, b, tolerance, simpson_rule(function, a, b, 2));
    }
    
private:
    double simpson_rule(const std::string& function, double a, double b, int n) {
        double h = (b - a) / n;
        double sum = 0.0;
        
        for (int i = 0; i <= n; ++i) {
            double x = a + i * h;
            parser_.set("x", x);
            double f_x = parser_.eval(function);
            
            if (i == 0 || i == n) {
                sum += f_x;
            } else if (i % 2 == 1) {
                sum += 4 * f_x;
            } else {
                sum += 2 * f_x;
            }
        }
        
        return sum * h / 3.0;
    }
    
    double adaptive_simpson(const std::string& function, double a, double b, 
                           double tolerance, double whole_area) {
        double c = (a + b) / 2.0;
        double left_area = simpson_rule(function, a, c, 2);
        double right_area = simpson_rule(function, c, b, 2);
        
        if (std::abs(left_area + right_area - whole_area) <= 15 * tolerance) {
            return left_area + right_area + (left_area + right_area - whole_area) / 15.0;
        }
        
        return adaptive_simpson(function, a, c, tolerance / 2.0, left_area) +
               adaptive_simpson(function, c, b, tolerance / 2.0, right_area);
    }
};

void demonstrate_numerical_methods() {
    NumericalMethods methods;
    
    std::cout << "=== Numerical Methods Examples ===" << std::endl;
    
    // Find root of x^2 - 2 = 0 (should be √2)
    try {
        double root = methods.find_root("x ** 2 - 2", "2 * x", 1.5);
        std::cout << "Root of x² - 2 = 0: " << root << " (√2 ≈ " << std::sqrt(2) << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Root finding failed: " << e.what() << std::endl;
    }
    
    // Find root of cos(x) = 0 (should be π/2)
    try {
        double root = methods.find_root("cos(x)", "-sin(x)", 1.0);
        std::cout << "Root of cos(x) = 0: " << root << " (π/2 ≈ " << M_PI/2 << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Root finding failed: " << e.what() << std::endl;
    }
    
    // Integrate x^2 from 0 to 1 (should be 1/3)
    double integral1 = methods.integrate("x ** 2", 0.0, 1.0);
    std::cout << "∫₀¹ x² dx = " << integral1 << " (exact: " << 1.0/3.0 << ")" << std::endl;
    
    // Integrate sin(x) from 0 to π (should be 2)
    double integral2 = methods.integrate("sin(x)", 0.0, M_PI);
    std::cout << "∫₀π sin(x) dx = " << integral2 << " (exact: 2)" << std::endl;
    
    // Integrate e^(-x²) from -∞ to ∞ (Gaussian integral, approximate with large bounds)
    double integral3 = methods.integrate("exp(-x ** 2)", -5.0, 5.0);
    std::cout << "∫₋₅⁵ e^(-x²) dx ≈ " << integral3 << " (√π ≈ " << std::sqrt(M_PI) << ")" << std::endl;
}
```

### Statistical Analysis System

Build a statistical analysis system with dynamic formula evaluation:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

class StatisticalAnalyzer {
private:
    calculator::ExpressionParser<double> parser_;
    std::vector<double> data_;
    
public:
    StatisticalAnalyzer() {
        setup_statistical_functions();
    }
    
    void setup_statistical_functions() {
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("exp", [](double x) { return std::exp(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        
        // Statistical functions
        parser_.set("factorial", [](double n) {
            if (n < 0 || n != std::floor(n)) return std::numeric_limits<double>::quiet_NaN();
            double result = 1.0;
            for (int i = 2; i <= static_cast<int>(n); ++i) {
                result *= i;
            }
            return result;
        });
        
        parser_.set("gamma", [](double x) {
            // Simplified gamma function approximation
            return std::tgamma(x);
        });
    }
    
    void set_data(const std::vector<double>& data) {
        data_ = data;
        update_statistics();
    }
    
    void update_statistics() {
        if (data_.empty()) return;
        
        // Basic statistics
        double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
        double mean = sum / data_.size();
        
        double variance = 0.0;
        for (double value : data_) {
            variance += (value - mean) * (value - mean);
        }
        variance /= data_.size();
        
        double std_dev = std::sqrt(variance);
        
        auto sorted_data = data_;
        std::sort(sorted_data.begin(), sorted_data.end());
        
        double median;
        size_t n = sorted_data.size();
        if (n % 2 == 0) {
            median = (sorted_data[n/2 - 1] + sorted_data[n/2]) / 2.0;
        } else {
            median = sorted_data[n/2];
        }
        
        // Set variables for formula evaluation
        parser_.set("n", static_cast<double>(data_.size()));
        parser_.set("mean", mean);
        parser_.set("variance", variance);
        parser_.set("std_dev", std_dev);
        parser_.set("median", median);
        parser_.set("min", *std::min_element(data_.begin(), data_.end()));
        parser_.set("max", *std::max_element(data_.begin(), data_.end()));
        parser_.set("sum", sum);
    }
    
    double evaluate_formula(const std::string& formula) {
        return parser_.eval(formula);
    }
    
    void print_summary() {
        std::cout << "Dataset size: " << parser_.eval("n") << std::endl;
        std::cout << "Mean: " << parser_.eval("mean") << std::endl;
        std::cout << "Median: " << parser_.eval("median") << std::endl;
        std::cout << "Standard deviation: " << parser_.eval("std_dev") << std::endl;
        std::cout << "Variance: " << parser_.eval("variance") << std::endl;
        std::cout << "Range: [" << parser_.eval("min") << ", " << parser_.eval("max") << "]" << std::endl;
    }
    
    // Calculate confidence interval for mean
    double confidence_interval_margin(double confidence_level) {
        // Simplified: assumes normal distribution
        parser_.set("confidence", confidence_level);
        
        // Z-score for common confidence levels (simplified)
        double z_score;
        if (confidence_level >= 0.99) z_score = 2.576;
        else if (confidence_level >= 0.95) z_score = 1.96;
        else if (confidence_level >= 0.90) z_score = 1.645;
        else z_score = 1.0;
        
        parser_.set("z_score", z_score);
        
        return parser_.eval("z_score * std_dev / sqrt(n)");
    }
    
    // Hypothesis testing
    double t_test_statistic(double hypothesized_mean) {
        parser_.set("h0_mean", hypothesized_mean);
        return parser_.eval("(mean - h0_mean) / (std_dev / sqrt(n))");
    }
};

void demonstrate_statistical_analysis() {
    StatisticalAnalyzer analyzer;
    
    // Sample dataset: test scores
    std::vector<double> test_scores = {
        85.5, 92.3, 78.1, 88.7, 91.2, 83.4, 89.6, 87.3, 90.1, 86.8,
        84.2, 93.5, 81.7, 88.9, 85.3, 90.7, 87.1, 89.4, 86.5, 88.2
    };
    
    analyzer.set_data(test_scores);
    
    std::cout << "=== Statistical Analysis Example ===" << std::endl;
    analyzer.print_summary();
    
    // Custom statistical calculations
    std::cout << "\nCustom Calculations:" << std::endl;
    std::cout << "Coefficient of variation: " << analyzer.evaluate_formula("std_dev / mean * 100") << "%" << std::endl;
    std::cout << "Z-score of max value: " << analyzer.evaluate_formula("(max - mean) / std_dev") << std::endl;
    std::cout << "Interquartile range estimate: " << analyzer.evaluate_formula("1.35 * std_dev") << std::endl;
    
    // Confidence intervals
    std::cout << "\nConfidence Intervals:" << std::endl;
    double margin_95 = analyzer.confidence_interval_margin(0.95);
    double margin_99 = analyzer.confidence_interval_margin(0.99);
    
    std::cout << "95% CI: [" << (analyzer.evaluate_formula("mean") - margin_95) 
              << ", " << (analyzer.evaluate_formula("mean") + margin_95) << "]" << std::endl;
    std::cout << "99% CI: [" << (analyzer.evaluate_formula("mean") - margin_99) 
              << ", " << (analyzer.evaluate_formula("mean") + margin_99) << "]" << std::endl;
    
    // Hypothesis testing
    double hypothesized_mean = 85.0;
    double t_stat = analyzer.t_test_statistic(hypothesized_mean);
    std::cout << "\nHypothesis Test (H₀: μ = " << hypothesized_mean << "):" << std::endl;
    std::cout << "t-statistic: " << t_stat << std::endl;
}
```

## Engineering Applications

### Control Systems Simulation

Implement control system analysis with transfer functions:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

class ControlSystemAnalyzer {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    ControlSystemAnalyzer() {
        setup_control_functions();
    }
    
    void setup_control_functions() {
        parser_.set("sin", [](double x) { return std::sin(x); });
        parser_.set("cos", [](double x) { return std::cos(x); });
        parser_.set("exp", [](double x) { return std::exp(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("pi", M_PI);
        parser_.set("e", M_E);
        
        // Control system specific functions
        parser_.set("step", [](double t) { return t >= 0 ? 1.0 : 0.0; });
        parser_.set("ramp", [](double t) { return t >= 0 ? t : 0.0; });
        parser_.set("impulse", [](double t) { return std::abs(t) < 1e-6 ? 1e6 : 0.0; });
    }
    
    // First-order system response: y(t) = K(1 - e^(-t/τ))
    double first_order_step_response(double time, double gain, double time_constant) {
        parser_.set("t", time);
        parser_.set("K", gain);
        parser_.set("tau", time_constant);
        
        return parser_.eval("K * (1 - exp(-t / tau))");
    }
    
    // Second-order system response
    double second_order_step_response(double time, double gain, double natural_freq, double damping_ratio) {
        parser_.set("t", time);
        parser_.set("K", gain);
        parser_.set("wn", natural_freq);
        parser_.set("zeta", damping_ratio);
        
        if (damping_ratio < 1.0) {
            // Underdamped
            parser_.set("wd", parser_.eval("wn * sqrt(1 - zeta ** 2)"));
            return parser_.eval("K * (1 - exp(-zeta * wn * t) * (cos(wd * t) + zeta / sqrt(1 - zeta ** 2) * sin(wd * t)))");
        } else if (damping_ratio == 1.0) {
            // Critically damped
            return parser_.eval("K * (1 - exp(-wn * t) * (1 + wn * t))");
        } else {
            // Overdamped
            parser_.set("r1", parser_.eval("wn * (-zeta + sqrt(zeta ** 2 - 1))"));
            parser_.set("r2", parser_.eval("wn * (-zeta - sqrt(zeta ** 2 - 1))"));
            return parser_.eval("K * (1 - (r2 * exp(r1 * t) - r1 * exp(r2 * t)) / (r2 - r1))");
        }
    }
    
    // PID controller output
    double pid_output(double error, double integral_error, double derivative_error,
                     double kp, double ki, double kd) {
        parser_.set("e", error);
        parser_.set("ei", integral_error);
        parser_.set("ed", derivative_error);
        parser_.set("Kp", kp);
        parser_.set("Ki", ki);
        parser_.set("Kd", kd);
        
        return parser_.eval("Kp * e + Ki * ei + Kd * ed");
    }
    
    // Bode plot magnitude calculation
    double bode_magnitude(double frequency, const std::string& transfer_function) {
        parser_.set("w", frequency);
        parser_.set("s", frequency); // For simplicity, using jω ≈ ω for magnitude
        
        double magnitude = parser_.eval(transfer_function);
        return 20 * std::log10(std::abs(magnitude));
    }
    
    void simulate_system_response() {
        std::cout << "=== Control System Simulation ===" << std::endl;
        
        // System parameters
        double gain = 2.0;
        double time_constant = 1.5;
        double natural_freq = 2.0;
        double damping_ratio = 0.7;
        
        std::cout << "First-order system (K=" << gain << ", τ=" << time_constant << "):" << std::endl;
        for (double t = 0; t <= 5.0; t += 1.0) {
            double response = first_order_step_response(t, gain, time_constant);
            std::cout << "t=" << t << "s: y=" << response << std::endl;
        }
        
        std::cout << "\nSecond-order system (K=" << gain << ", ωₙ=" << natural_freq 
                  << ", ζ=" << damping_ratio << "):" << std::endl;
        for (double t = 0; t <= 3.0; t += 0.5) {
            double response = second_order_step_response(t, gain, natural_freq, damping_ratio);
            std::cout << "t=" << t << "s: y=" << response << std::endl;
        }
        
        // PID controller simulation
        std::cout << "\nPID Controller simulation:" << std::endl;
        double kp = 1.0, ki = 0.5, kd = 0.1;
        double setpoint = 1.0;
        double output = 0.0;
        double integral = 0.0;
        double previous_error = 0.0;
        double dt = 0.1;
        
        for (int i = 0; i < 10; ++i) {
            double error = setpoint - output;
            integral += error * dt;
            double derivative = (error - previous_error) / dt;
            
            double control_signal = pid_output(error, integral, derivative, kp, ki, kd);
            
            // Simple plant model: first-order system
            output += (control_signal - output) * dt / time_constant;
            
            std::cout << "Step " << i << ": error=" << error << ", output=" << output 
                      << ", control=" << control_signal << std::endl;
            
            previous_error = error;
        }
    }
};

void demonstrate_control_systems() {
    ControlSystemAnalyzer analyzer;
    analyzer.simulate_system_response();
}
```

## Financial Modeling

### Advanced Financial Calculator

Implement sophisticated financial calculations:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <cmath>

class FinancialCalculator {
private:
    calculator::ExpressionParser<double> parser_;
    
public:
    FinancialCalculator() {
        setup_financial_functions();
    }
    
    void setup_financial_functions() {
        parser_.set("exp", [](double x) { return std::exp(x); });
        parser_.set("log", [](double x) { return std::log(x); });
        parser_.set("sqrt", [](double x) { return std::sqrt(x); });
        parser_.set("abs", [](double x) { return std::abs(x); });
        parser_.set("max", [](double x) { return std::max(x, 0.0); });
        parser_.set("min", [](double x) { return std::min(x, 1.0); });
        parser_.set("e", M_E);
        
        // Financial functions
        parser_.set("pv_factor", [](double rate) {
            // Present value factor: 1/(1+r)^n, simplified for single period
            return 1.0 / (1.0 + rate);
        });
    }
    
    // Black-Scholes option pricing
    double black_scholes_call(double S, double K, double T, double r, double sigma) {
        parser_.set("S", S);    // Current stock price
        parser_.set("K", K);    // Strike price
        parser_.set("T", T);    // Time to expiration
        parser_.set("r", r);    // Risk-free rate
        parser_.set("sigma", sigma); // Volatility
        
        // Calculate d1 and d2
        double d1 = parser_.eval("(log(S / K) + (r + sigma ** 2 / 2) * T) / (sigma * sqrt(T))");
        double d2 = parser_.eval("(log(S / K) + (r - sigma ** 2 / 2) * T) / (sigma * sqrt(T))");
        
        parser_.set("d1", d1);
        parser_.set("d2", d2);
        
        // Simplified normal CDF approximation
        auto norm_cdf = [](double x) {
            return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
        };
        
        double N_d1 = norm_cdf(d1);
        double N_d2 = norm_cdf(d2);
        
        parser_.set("N_d1", N_d1);
        parser_.set("N_d2", N_d2);
        
        return parser_.eval("S * N_d1 - K * exp(-r * T) * N_d2");
    }
    
    // Monte Carlo option pricing
    double monte_carlo_option_price(double S0, double K, double T, double r, double sigma, 
                                   int num_simulations = 100000) {
        parser_.set("S0", S0);
        parser_.set("K", K);
        parser_.set("T", T);
        parser_.set("r", r);
        parser_.set("sigma", sigma);
        
        double payoff_sum = 0.0;
        
        for (int i = 0; i < num_simulations; ++i) {
            // Generate random normal variable (Box-Muller transform)
            static bool has_spare = false;
            static double spare;
            
            double z;
            if (has_spare) {
                z = spare;
                has_spare = false;
            } else {
                double u1 = static_cast<double>(rand()) / RAND_MAX;
                double u2 = static_cast<double>(rand()) / RAND_MAX;
                double mag = sigma * std::sqrt(-2.0 * std::log(u1));
                z = mag * std::cos(2.0 * M_PI * u2);
                spare = mag * std::sin(2.0 * M_PI * u2);
                has_spare = true;
            }
            
            parser_.set("z", z);
            
            // Stock price at expiration
            double ST = parser_.eval("S0 * exp((r - sigma ** 2 / 2) * T + sigma * sqrt(T) * z)");
            
            // Option payoff
            double payoff = std::max(ST - K, 0.0);
            payoff_sum += payoff;
        }
        
        double average_payoff = payoff_sum / num_simulations;
        parser_.set("avg_payoff", average_payoff);
        
        return parser_.eval("exp(-r * T) * avg_payoff");
    }
    
    // Portfolio optimization (simplified mean-variance)
    struct PortfolioResult {
        std::vector<double> weights;
        double expected_return;
        double risk;
        double sharpe_ratio;
    };
    
    PortfolioResult optimize_portfolio(const std::vector<double>& expected_returns,
                                      const std::vector<std::vector<double>>& covariance_matrix,
                                      double risk_free_rate) {
        // Simplified: equal weight portfolio for demonstration
        size_t n = expected_returns.size();
        std::vector<double> weights(n, 1.0 / n);
        
        // Calculate portfolio expected return
        double portfolio_return = 0.0;
        for (size_t i = 0; i < n; ++i) {
            portfolio_return += weights[i] * expected_returns[i];
        }
        
        // Calculate portfolio variance
        double portfolio_variance = 0.0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                portfolio_variance += weights[i] * weights[j] * covariance_matrix[i][j];
            }
        }
        
        double portfolio_risk = std::sqrt(portfolio_variance);
        
        parser_.set("port_return", portfolio_return);
        parser_.set("port_risk", portfolio_risk);
        parser_.set("rf_rate", risk_free_rate);
        
        double sharpe_ratio = parser_.eval("(port_return - rf_rate) / port_risk");
        
        return {weights, portfolio_return, portfolio_risk, sharpe_ratio};
    }
    
    void demonstrate_financial_models() {
        std::cout << "=== Advanced Financial Modeling ===" << std::endl;
        
        // Black-Scholes option pricing
        double S = 100.0;  // Current stock price
        double K = 105.0;  // Strike price
        double T = 0.25;   // 3 months to expiration
        double r = 0.05;   // 5% risk-free rate
        double sigma = 0.2; // 20% volatility
        
        double bs_price = black_scholes_call(S, K, T, r, sigma);
        std::cout << "Black-Scholes call option price: $" << bs_price << std::endl;
        
        // Monte Carlo pricing
        srand(42); // For reproducible results
        double mc_price = monte_carlo_option_price(S, K, T, r, sigma, 10000);
        std::cout << "Monte Carlo call option price: $" << mc_price << std::endl;
        std::cout << "Difference: $" << std::abs(bs_price - mc_price) << std::endl;
        
        // Portfolio optimization
        std::vector<double> returns = {0.12, 0.10, 0.08, 0.15};
        std::vector<std::vector<double>> covariance = {
            {0.04, 0.02, 0.01, 0.03},
            {0.02, 0.03, 0.015, 0.025},
            {0.01, 0.015, 0.02, 0.02},
            {0.03, 0.025, 0.02, 0.05}
        };
        
        auto portfolio = optimize_portfolio(returns, covariance, 0.03);
        
        std::cout << "\nPortfolio Optimization Results:" << std::endl;
        std::cout << "Expected Return: " << (portfolio.expected_return * 100) << "%" << std::endl;
        std::cout << "Risk (Std Dev): " << (portfolio.risk * 100) << "%" << std::endl;
        std::cout << "Sharpe Ratio: " << portfolio.sharpe_ratio << std::endl;
        
        std::cout << "Weights: ";
        for (size_t i = 0; i < portfolio.weights.size(); ++i) {
            std::cout << "Asset " << (i+1) << ": " << (portfolio.weights[i] * 100) << "% ";
        }
        std::cout << std::endl;
    }
};

void demonstrate_financial_modeling() {
    FinancialCalculator calc;
    calc.demonstrate_financial_models();
}
```

## Performance Optimization Examples

### High-Performance Batch Processing

Implement optimized batch processing for large-scale calculations:

```cpp
#include "calculator.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <memory>

class HighPerformanceCalculator {
private:
    std::vector<std::unique_ptr<calculator::ExpressionParser<double>>> parsers_;
    
public:
    HighPerformanceCalculator(size_t num_threads = std::thread::hardware_concurrency()) {
        // Create one parser per thread to avoid synchronization
        for (size_t i = 0; i < num_threads; ++i) {
            auto parser = std::make_unique<calculator::ExpressionParser<double>>();
            setup_parser(*parser);
            parsers_.push_back(std::move(parser));
        }
    }
    
    void setup_parser(calculator::ExpressionParser<double>& parser) {
        parser.set("sin", [](double x) { return std::sin(x); });
        parser.set("cos", [](double x) { return std::cos(x); });
        parser.set("sqrt", [](double x) { return std::sqrt(x); });
        parser.set("log", [](double x) { return std::log(x); });
        parser.set("exp", [](double x) { return std::exp(x); });
        parser.set("pi", M_PI);
        parser.set("e", M_E);
    }
    
    // Sequential processing
    std::vector<double> process_sequential(const std::vector<std::string>& expressions) {
        std::vector<double> results;
        results.reserve(expressions.size());
        
        auto& parser = *parsers_[0];
        
        for (const auto& expr : expressions) {
            try {
                results.push_back(parser.eval(expr));
            } catch (const calculator::error&) {
                results.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }
        
        return results;
    }
    
    // Parallel processing
    std::vector<double> process_parallel(const std::vector<std::string>& expressions) {
        const size_t num_threads = parsers_.size();
        const size_t chunk_size = (expressions.size() + num_threads - 1) / num_threads;
        
        std::vector<std::future<std::vector<double>>> futures;
        
        for (size_t thread_id = 0; thread_id < num_threads; ++thread_id) {
            size_t start = thread_id * chunk_size;
            size_t end = std::min(start + chunk_size, expressions.size());
            
            if (start >= expressions.size()) break;
            
            auto chunk = std::vector<std::string>(expressions.begin() + start, expressions.begin() + end);
            
            futures.push_back(std::async(std::launch::async, [this, thread_id, chunk]() {
                std::vector<double> chunk_results;
                chunk_results.reserve(chunk.size());
                
                auto& parser = *parsers_[thread_id];
                
                for (const auto& expr : chunk) {
                    try {
                        chunk_results.push_back(parser.eval(expr));
                    } catch (const calculator::error&) {
                        chunk_results.push_back(std::numeric_limits<double>::quiet_NaN());
                    }
                }
                
                return chunk_results;
            }));
        }
        
        std::vector<double> results;
        results.reserve(expressions.size());
        
        for (auto& future : futures) {
            auto chunk_results = future.get();
            results.insert(results.end(), chunk_results.begin(), chunk_results.end());
        }
        
        return results;
    }
    
    void benchmark_performance() {
        // Generate test expressions
        std::vector<std::string> expressions;
        expressions.reserve(100000);
        
        for (int i = 0; i < 100000; ++i) {
            double x = i * 0.001;
            expressions.push_back("sin(" + std::to_string(x) + ") + cos(" + std::to_string(x) + ")");
        }
        
        std::cout << "=== Performance Benchmark ===" << std::endl;
        std::cout << "Processing " << expressions.size() << " expressions..." << std::endl;
        
        // Sequential benchmark
        auto start = std::chrono::high_resolution_clock::now();
        auto seq_results = process_sequential(expressions);
        auto end = std::chrono::high_resolution_clock::now();
        auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Parallel benchmark
        start = std::chrono::high_resolution_clock::now();
        auto par_results = process_parallel(expressions);
        end = std::chrono::high_resolution_clock::now();
        auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "Sequential time: " << seq_time.count() << " ms" << std::endl;
        std::cout << "Parallel time: " << par_time.count() << " ms" << std::endl;
        std::cout << "Speedup: " << (double)seq_time.count() / par_time.count() << "x" << std::endl;
        std::cout << "Threads used: " << parsers_.size() << std::endl;
        
        // Verify results are identical
        bool results_match = true;
        for (size_t i = 0; i < seq_results.size(); ++i) {
            if (std::abs(seq_results[i] - par_results[i]) > 1e-10) {
                results_match = false;
                break;
            }
        }
        
        std::cout << "Results match: " << (results_match ? "Yes" : "No") << std::endl;
    }
};

void demonstrate_performance_optimization() {
    HighPerformanceCalculator calc;
    calc.benchmark_performance();
}
```

## Main Demonstration Function

```cpp
int main() {
    std::cout << "Modern C++ Calculator - Advanced Examples\n" << std::endl;
    
    try {
        demonstrate_numerical_methods();
        std::cout << std::endl;
        
        demonstrate_statistical_analysis();
        std::cout << std::endl;
        
        demonstrate_control_systems();
        std::cout << std::endl;
        
        demonstrate_financial_modeling();
        std::cout << std::endl;
        
        demonstrate_performance_optimization();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Compilation Instructions

To compile these advanced examples:

```bash
g++ -std=c++17 -O3 -pthread advanced_examples.cpp -o advanced_examples
```

## Key Takeaways

These advanced examples demonstrate:

1. **Numerical Methods**: Root finding, integration, and scientific computing
2. **Statistical Analysis**: Data analysis with dynamic formula evaluation
3. **Control Systems**: Engineering simulations and system analysis
4. **Financial Modeling**: Option pricing and portfolio optimization
5. **Performance Optimization**: Parallel processing and benchmarking

Each example showcases how the calculator library can be integrated into sophisticated mathematical and engineering applications, providing both flexibility and performance.

## Next Steps

- Explore [Real-world Usage](real-world-usage.md) for production applications
- Review [User Guide](../user-guide/advanced-features.md) for optimization techniques
- Check [API Reference](../api-reference/) for complete documentation
- See [Basic Examples](basic-examples.md) for fundamental patterns
