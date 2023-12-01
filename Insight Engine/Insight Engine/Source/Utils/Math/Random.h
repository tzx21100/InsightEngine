/*!
 * \file Random.h
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 28-09-2023
 * \brief
 * This header file defines the PRNG class for random number generation.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/


#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MATH_RANDOM_H
#define GAM200_INSIGHT_ENGINE_SOURCE_MATH_RANDOM_H

#include <random>

namespace IS {

    class PRNG {
    public:
        static PRNG& Instance() {
            static PRNG prng;
            return prng;
        }

        float generate() {
            return distribution(generator);
        }
    private:
        PRNG() {
            std::random_device rd;
            generator = std::mt19937(rd());
            distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
        }

        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution;
    };

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_MATH_RANDOM_H
