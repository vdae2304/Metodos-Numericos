/*
 * This file is part of the NumCpp project.
 *
 * NumCPP is a package for scientific computing in C++. It is a C++ library
 * that provides support for multidimensional arrays, and defines an assortment
 * of routines for fast operations on them, including mathematical, logical,
 * sorting, selecting, I/O and much more.
 *
 * NumCPP comes from Numeric C++ and, as the name suggests, is a package
 * inspired by the NumPy package for Python, although it is completely
 * independent from its Python counterpart.
 *
 * This program is free software: you can redistribute it and/or modify it by
 * giving enough credit to its creators.
 */

 /** @file include/numcpp/random/distributions.h
  *  This is an internal header file, included by other library headers.
  *  Do not attempt to use it directly. @headername{numcpp/random.h}
  */

 // Written by Victor Daniel Alvarado Estrella (https://github.com/vdae2304).

#ifndef NUMCPP_DISTRIBUTIONS_H_INCLUDED
#define NUMCPP_DISTRIBUTIONS_H_INCLUDED

#include <cmath>
#include <limits>
#include <random>

namespace numcpp {
    /// Discrete distributions
    using std::uniform_int_distribution;
    using std::discrete_distribution;
    using std::binomial_distribution;
    using std::geometric_distribution;
    using std::negative_binomial_distribution;
    using std::poisson_distribution;

    /// Continuous distributions
    using std::uniform_real_distribution;
    using std::piecewise_linear_distribution;
    using std::cauchy_distribution;
    using std::chi_squared_distribution;
    using std::exponential_distribution;
    using std::fisher_f_distribution;
    using std::gamma_distribution;
    using std::extreme_value_distribution;
    using std::lognormal_distribution;
    using std::normal_distribution;
    using std::student_t_distribution;
    using std::weibull_distribution;

/// Namespace for implementation details.
namespace detail {
    /**
     * @brief Helper function. Generates a uniform random floating point number
     * in [0, 1).
     */
    template <class RealType, class UniformRandomNumberGenerator>
    RealType random(UniformRandomNumberGenerator &urng) {
        return std::generate_canonical<
            RealType, std::numeric_limits<RealType>::digits
        >(urng);
    }
}

    /**
     * @brief A beta continuous distribution for random numbers.
     *
     * The formula for the beta probability density function is
     * @f[
     *     f(x;\alpha,\beta) = \frac{\Gamma(\alpha+\beta)}
     *         {\Gamma(\alpha)\Gamma(\beta)} x^{\alpha-1} (1-x)^{\beta-1}
     * @f]
     */
    template <class RealType = double>
    class beta_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef beta_distribution<RealType> distribution_type;

            param_type() : m_alpha(1.0), m_beta(1.0) {}

            param_type(RealType alpha, RealType beta = RealType(1))
             : m_alpha(alpha), m_beta(beta) {}

            RealType alpha() const {
                return m_alpha;
            }

            RealType beta() const {
                return m_beta;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_alpha == p2.m_alpha && p1.m_beta == p2.m_beta);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_alpha;
            RealType m_beta;
        };

        /**
         * @brief Constructs a beta distribution with parameters 1 and 1.
         */
        beta_distribution() : m_param() {}

        /**
         * @brief Constructs a beta distribution with parameters @f$\alpha@f$
         * and @f$\beta@f$.
         */
        beta_distribution(RealType alpha, RealType beta = RealType(1))
         : m_param(alpha, beta) {}

        beta_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {
            __gamma_rvs.reset();
        }

        /**
         * @brief Returns the @f$\alpha@f$ of the distribution.
         */
        RealType alpha() const {
            return m_param.alpha();
        }

        /**
         * @brief Returns the @f$\beta@f$ of the distribution.
         */
        RealType beta() const {
            return m_param.beta();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return RealType(0);
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return RealType(1);
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two beta distributions have the same parameters
         * and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const beta_distribution &d1, const beta_distribution &d2
        ) {
            return (d1.m_param == d2.m_param &&
                    d1.__gamma_rvs == d2.__gamma_rvs);
        }

        /**
         * @brief Return true if two beta distributions are different.
         */
        friend bool operator!=(
            const beta_distribution &d1, const beta_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;

        std::gamma_distribution<RealType> __gamma_rvs;
    };

    /**
     * @brief An inverse Gaussian continuous distribution for random numbers.
     *
     * The formula for the inverse Gaussian probability density function is
     * @f[
     *     f(x;\mu,\lambda) = \sqrt{\frac{\lambda}{2\pi x^3}}
     *         \exp\left(-\frac{\lambda(x-\mu)^2}{2\mu^2 x}\right)
     * @f]
     */
    template <class RealType = double>
    class inverse_gaussian_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef inverse_gaussian_distribution<RealType> distribution_type;

            param_type() : m_mu(1.0), m_lambda(1.0) {}

            param_type(RealType mu, RealType lambda = RealType(1))
             : m_mu(mu), m_lambda(lambda) {}

            RealType mu() const {
                return m_mu;
            }

            RealType lambda() const {
                return m_lambda;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_mu == p2.m_mu && p1.m_lambda == p2.m_lambda);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_mu;
            RealType m_lambda;
        };

        /**
         * @brief Constructs an inverse Gaussian distribution with parameters 1
         * and 1.
         */
        inverse_gaussian_distribution() : m_param() {}

        /**
         * @brief Constructs an inverse Gaussian distribution with parameters
         * @f$\mu@f$ and @f$\lambda@f$.
         */
        inverse_gaussian_distribution(RealType mu, RealType lambda=RealType(1))
         : m_param(mu, lambda) {}

        inverse_gaussian_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {
            __normal_rvs.reset();
        }

        /**
         * @brief Returns the @f$\mu@f$ of the distribution.
         */
        RealType mu() const {
            return m_param.mu();
        }

        /**
         * @brief Returns the @f$\lambda@f$ of the distribution.
         */
        RealType lambda() const {
            return m_param.lambda();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return RealType(0);
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return std::numeric_limits<RealType>::max();
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two inverse Gaussian distributions have the
         * same parameters and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const inverse_gaussian_distribution &d1,
            const inverse_gaussian_distribution &d2
        ) {
            return (d1.m_param == d2.m_param &&
                    d1.__normal_rvs == d2.__normal_rvs);
        }

        /**
         * @brief Return true if two inverse Gaussian distributions are
         * different.
         */
        friend bool operator!=(
            const inverse_gaussian_distribution &d1,
            const inverse_gaussian_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;

        std::normal_distribution<RealType> __normal_rvs;
    };

    /**
     * @brief A Laplace continuous distribution for random numbers.
     *
     * The formula for the Laplace probability density function is
     * @f[
     *     f(x;\mu,s) = \frac{1}{2s}\exp\left(-\frac{|x-\mu|}{s}\right)
     * @f]
     */
    template <class RealType = double>
    class laplace_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef laplace_distribution<RealType> distribution_type;

            param_type() : m_mu(0.0), m_s(1.0) {}

            param_type(RealType mu, RealType s = RealType(1))
             : m_mu(mu), m_s(s) {}

            RealType mu() const {
                return m_mu;
            }

            RealType s() const {
                return m_s;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_mu == p2.m_mu && p1.m_s == p2.m_s);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_mu;
            RealType m_s;
        };

        /**
         * @brief Constructs a Laplace distribution with parameters 0 and 1.
         */
        laplace_distribution() : m_param() {}

        /**
         * @brief Constructs a Laplace distribution with parameters @f$\mu@f$
         * and @f$s@f$.
         */
        laplace_distribution(RealType mu, RealType s = RealType(1))
         : m_param(mu, s) {}

        laplace_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {}

        /**
         * @brief Returns the @f$\mu@f$ of the distribution.
         */
        RealType mu() const {
            return m_param.mu();
        }

        /**
         * @brief Returns the @f$s@f$ of the distribution.
         */
        RealType s() const {
            return m_param.s();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return std::numeric_limits<RealType>::min();
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return std::numeric_limits<RealType>::max();
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two Laplace distributions have the same
         * parameters and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const laplace_distribution &d1, const laplace_distribution &d2
        ) {
            return (d1.m_param == d2.m_param);
        }

        /**
         * @brief Return true if two Laplace distributions are different.
         */
        friend bool operator!=(
            const laplace_distribution &d1, const laplace_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;
    };

    /**
     * @brief A logistic continuous distribution for random numbers.
     *
     * The formula for the logistic probability density function is
     * @f[
     *     f(x;\mu,s) = \frac{e^{-(x-\mu)/2}}{s\left(1+e^{-(x-\mu)/s}\right)^2}
     * @f]
     */
    template <class RealType = double>
    class logistic_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef logistic_distribution<RealType> distribution_type;

            param_type() : m_mu(0.0), m_s(1.0) {}

            param_type(RealType mu, RealType s = RealType(1))
             : m_mu(mu), m_s(s) {}

            RealType mu() const {
                return m_mu;
            }

            RealType s() const {
                return m_s;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_mu == p2.m_mu && p1.m_s == p2.m_s);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_mu;
            RealType m_s;
        };

        /**
         * @brief Constructs a logistic distribution with parameters 0 and 1.
         */
        logistic_distribution() : m_param() {}

        /**
         * @brief Constructs a logistic distribution with parameters @f$\mu@f$
         * and @f$s@f$.
         */
        logistic_distribution(RealType mu, RealType s = RealType(1))
         : m_param(mu, s) {}

        logistic_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {}

        /**
         * @brief Returns the @f$\mu@f$ of the distribution.
         */
        RealType mu() const {
            return m_param.mu();
        }

        /**
         * @brief Returns the @f$s@f$ of the distribution.
         */
        RealType s() const {
            return m_param.s();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return std::numeric_limits<RealType>::min();
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return std::numeric_limits<RealType>::max();
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two logistic distributions have the same
         * parameters and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const logistic_distribution &d1, const logistic_distribution &d2
        ) {
            return (d1.m_param == d2.m_param);
        }

        /**
         * @brief Return true if two logistic distributions are different.
         */
        friend bool operator!=(
            const logistic_distribution &d1, const logistic_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;
    };

    /**
     * @brief A Pareto continuous distribution for random numbers.
     *
     * The formula for the Pareto probability density function is
     * @f[
     *     f(x;\alpha,x_m) = \frac{\alpha x_m^{\alpha}}{x^{\alpha+1}}
     * @f]
     */
    template <class RealType = double>
    class pareto_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef pareto_distribution<RealType> distribution_type;

            param_type() : m_alpha(1.0), m_xm(1.0) {}

            param_type(RealType alpha, RealType xm = RealType(1))
             : m_alpha(alpha), m_xm(xm) {}

            RealType alpha() const {
                return m_alpha;
            }

            RealType xm() const {
                return m_xm;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_alpha == p2.m_alpha && p1.m_xm == p2.m_xm);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_alpha;
            RealType m_xm;
        };

        /**
         * @brief Constructs a Pareto distribution with parameters 1 and 1.
         */
        pareto_distribution() : m_param() {}

        /**
         * @brief Constructs a Pareto distribution with parameters @f$\mu@f$
         * and @f$s@f$.
         */
        pareto_distribution(RealType alpha, RealType xm = RealType(1))
         : m_param(alpha, xm) {}

        pareto_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {}

        /**
         * @brief Returns the @f$\alpha@f$ of the distribution.
         */
        RealType alpha() const {
            return m_param.alpha();
        }

        /**
         * @brief Returns the @f$x_m@f$ of the distribution.
         */
        RealType xm() const {
            return m_param.xm();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return m_param.xm();
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return std::numeric_limits<RealType>::max();
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two Pareto distributions have the same
         * parameters and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const pareto_distribution &d1, const pareto_distribution &d2
        ) {
            return (d1.m_param == d2.m_param);
        }

        /**
         * @brief Return true if two Pareto distributions are different.
         */
        friend bool operator!=(
            const pareto_distribution &d1, const pareto_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;
    };

    /**
     * @brief A Rayleigh continuous distribution for random numbers.
     *
     * The formula for the Rayleigh probability density function is
     * @f[
     *     f(x;\sigma) = \frac{x}{\sigma^2}e^{-x^2/(2\sigma^2)}
     * @f]
     */
    template <class RealType = double>
    class rayleigh_distribution {
        static_assert(std::is_floating_point<RealType>::value,
                      "result_type must be a floating point type");

    public:
        // The type of the range of the distribution.
        typedef RealType result_type;

        // Parameter type.
        struct param_type {
            typedef rayleigh_distribution<RealType> distribution_type;

            param_type() : m_sigma(1.0) {}

            param_type(RealType sigma) : m_sigma(sigma) {}

            RealType sigma() const {
                return m_sigma;
            }

            friend bool operator==(const param_type &p1, const param_type &p2) {
                return (p1.m_sigma == p2.m_sigma);
            }

            friend bool operator!=(const param_type &p1, const param_type &p2) {
                return !(p1 == p2);
            }

        private:
            RealType m_sigma;
        };

        /**
         * @brief Constructs a Rayleigh distribution with parameter 1.
         */
        rayleigh_distribution() : m_param() {}

        /**
         * @brief Constructs a Rayleigh distribution with parameter
         * @f$\sigma@f$.
         */
        rayleigh_distribution(RealType sigma) : m_param(sigma) {}

        rayleigh_distribution(const param_type &p) : m_param(p) {}

        /**
         * @brief Resets the distribution state.
         */
        void reset() {}

        /**
         * @brief Returns the @f$\sigma@f$ of the distribution.
         */
        RealType sigma() const {
            return m_param.sigma();
        }

        /**
         * @brief Returns the parameter set of the distribution.
         */
        param_type param() const {
            return m_param;
        }

        /**
         * @brief Sets the parameter set of the distribution.
         */
        void param(const param_type &p) {
            m_param = p;
        }

        /**
         * @brief Returns the greatest lower bound value of the distribution.
         */
        RealType min() const {
            return RealType(0);
        }

        /**
         * @brief Returns the least upper bound value of the distribution.
         */
        RealType max() const {
            return std::numeric_limits<RealType>::max();
        }

        /**
         * @brief Generating functions.
         */
        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng) {
            return this->operator()(urng, m_param);
        }

        template <class UniformRandomGenerator>
        RealType operator()(UniformRandomGenerator &urng, const param_type &p);

        /**
         * @brief Return true if two Rayleigh distributions have the same
         * parameters and the sequences that would be generated are equal.
         */
        friend bool operator==(
            const rayleigh_distribution &d1, const rayleigh_distribution &d2
        ) {
            return (d1.m_param == d2.m_param);
        }

        /**
         * @brief Return true if two Rayleigh distributions are different.
         */
        friend bool operator!=(
            const rayleigh_distribution &d1, const rayleigh_distribution &d2
        ) {
            return !(d1 == d2);
        }

    private:
        param_type m_param;
    };

    template <class RealType>
    template <class UniformRandomGenerator>
    RealType beta_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        typedef typename std::gamma_distribution<RealType>::param_type
            gamma_param_type;
        if (p.alpha() <= 1.0 && p.beta() <= 1.0) {
            // Use Johnk's algorithm.
            RealType U, V, X, Y;
            do {
                U = detail::random<RealType>(urng);
                V = detail::random<RealType>(urng);
                X = std::pow(U, 1.0 / p.alpha());
                Y = std::pow(V, 1.0 / p.beta());
            } while (X + Y > 1.0 || X + Y == 0.0);
            return X / (X + Y);
        }
        else {
            RealType X = __gamma_rvs(urng, gamma_param_type(p.alpha()));
            RealType Y = __gamma_rvs(urng, gamma_param_type(p.beta()));
            return X / (X + Y);
        }
    }

    template<class RealType>
    template <class UniformRandomGenerator>
    RealType inverse_gaussian_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        RealType c = p.mu() / (2.0 * p.lambda());
        RealType Y = __normal_rvs(urng);
        Y = p.mu() * Y * Y;
        RealType X = p.mu() + c * (Y - std::sqrt(4.0 * p.lambda() * Y + Y * Y));
        RealType U = detail::random<RealType>(urng);
        if (U <= p.mu() / (p.mu() + X)) {
            return X;
        }
        else {
            return p.mu() * p.mu() / X;
        }
    }

    template <class RealType>
    template <class UniformRandomGenerator>
    RealType laplace_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        // Use inverse CDF.
        RealType U;
        do {
            U = detail::random<RealType>(urng);
        } while (U == 0.0);
        if (U <= 0.5) {
            return p.mu() + p.s() * std::log(2.0 * U);
        }
        else {
            return p.mu() - p.s() * std::log(2.0 * (1.0 - U));
        }
    }

    template <class RealType>
    template <class UniformRandomGenerator>
    RealType logistic_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        // Use inverse CDF.
        RealType U;
        do {
            U = detail::random<RealType>(urng);
        } while (U == 0.0);
        return p.mu() + p.s() * std::log(U / (1.0 - U));
    }

    template <class RealType>
    template <class UniformRandomGenerator>
    RealType pareto_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        // Use inverse CDF.
        RealType U = detail::random<RealType>(urng);
        return p.xm() * std::pow(1.0 - U, -1.0 / p.alpha());
    }

    template <class RealType>
    template <class UniformRandomGenerator>
    RealType rayleigh_distribution<RealType>::operator()(
        UniformRandomGenerator &urng, const param_type &p
    ) {
        // Use inverse CDF.
        RealType U = detail::random<RealType>(urng);
        return p.sigma() * std::sqrt(-2.0 * std::log1p(-U));
    }
}

#endif // NUMCPP_DISTRIBUTIONS_H_INCLUDED
