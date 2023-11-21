/*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "System.h"
#include <array>
#include <fstream>
#include <sstream>
#define MAX_PARTICLES 10000


namespace IS {


	enum Default_Colors : int {
		color_red = 0,
		color_green = 1,
		color_blue = 2,
		color_white = 3,
		color_black = 4
	};

	enum Particle_Type : int {
		pt_square = 0,
		pt_circle = 1,
		pt_texture = 2,

	};

	class Color {

	public:

		Color(float red, float green, float blue, float alpha) :R{ red }, G{ green }, B{ blue }, A{ alpha } {};

		Color(Default_Colors color) {
			switch (color)
			{

			case color_red:
				R = 1.f; G = 0.f; B = 0.f; A = 1.f;
				break;

			case color_green:
				R = 0.f; G = 1.f; B = 0.f; A = 1.f;
				break;

			case color_blue:
				R = 0.f; G = 0.f; B = 1.f; A = 1.f;
				break;

			case color_white:
				R = 1.f; G = 1.f; B = 1.f; A = 1.f;
				break;

			case color_black:
				R = 0.f; G = 0.f; B = 0.f; A = 1.f;
				break;

			default:
				R = 1.f; G = 1.f; B = 1.f; A = 1.f;
				break;


			}

		}

		float R{ 1.f }, G{ 1.f }, B{ 1.f }, A{ 1.f };


		// Serialize Color object to string
		std::string Serialize() const {
			std::ostringstream out;
			out << R << "," << G << "," << B << "," << A;
			return out.str();
		}

		// Static method to deserialize a string to a Color object
		static Color Deserialize(const std::string& data) {
			std::istringstream in(data);
			float r, g, b, a;
			char delimiter;

			in >> r >> delimiter >> g >> delimiter >> b >> delimiter >> a;
			return Color(r, g, b, a);
		}



	};




	struct Particle {

		// Particle Attributes
		Vec2D mVelocity = { 0.f,0.f };
		float mSizeGrowth = 0.f;
		Vector2D mScale = { 1.f ,1.f };
		float mLifespan = 1.f;
		Color mColor = color_white;
		float mAlpha = 1.f;
		float mAlphaGrowth = 0.f;
		float mRotation = 0.f;
		float mDirection = 0.f;
		Particle_Type mParticleType = pt_square;
		std::string mImageName = "";

		Vector2D mParticlePos = { 0.f,0.f };

		static Particle CreateParticle(const Vec2D& velocity, float sizeGrowth,
			const Vector2D& scale, float lifespan,
			const Color& color, float alpha, float alphaGrowth,
			float rotation, float direction, Particle_Type particleType,
			const std::string& imageName, const Vector2D& particlePos) {
			Particle newParticle;
			newParticle.mVelocity = velocity;
			newParticle.mSizeGrowth = sizeGrowth;
			newParticle.mScale = scale;
			newParticle.mLifespan = lifespan;
			newParticle.mColor = color;
			newParticle.mAlpha = alpha;
			newParticle.mAlphaGrowth = alphaGrowth;
			newParticle.mRotation = rotation;
			newParticle.mDirection = direction;
			newParticle.mParticleType = particleType;
			newParticle.mImageName = imageName;
			newParticle.mParticlePos = particlePos;

			return newParticle;
		}

		// Serialize Particle object to a string with labels
		std::string Serialize() const {
			std::ostringstream out;
			out << "Velocity: " << mVelocity.x << ", " << mVelocity.y << "\n"
				<< "Size Growth: " << mSizeGrowth << "\n"
				<< "Scale: " << mScale.x << ", " << mScale.y << "\n"
				<< "Lifespan: " << mLifespan << "\n"
				<< "Color:\n" << mColor.Serialize()
				<< "Alpha: " << mAlpha << "\n"
				<< "Alpha Growth: " << mAlphaGrowth << "\n"
				<< "Rotation: " << mRotation << "\n"
				<< "Direction: " << mDirection << "\n"
				<< "Particle Type: " << mParticleType << "\n"
				<< "Image Name: " << mImageName << "\n"
				<< "Particle Position: " << mParticlePos.x << ", " << mParticlePos.y << "\n";
			return out.str();
		}

		// Deserialize a string to a Particle object
		static Particle Deserialize(const std::string& data) {
			std::istringstream in(data);
			std::string label;
			Particle particle;

			in >> label >> particle.mVelocity.x >> label >> particle.mVelocity.y
				>> label >> particle.mSizeGrowth
				>> label >> particle.mScale.x >> label >> particle.mScale.y
				>> label >> particle.mLifespan;

			in.ignore(); // Skip to the next line for Color
			std::string color;
			in >> color;
			particle.mColor = Color::Deserialize(color);

			int particle_type;

			in >> label >> particle.mAlpha
				>> label >> particle.mAlphaGrowth
				>> label >> particle.mRotation		
				>> label >> particle.mDirection
				>> label >> particle_type
				>> label;
			std::getline(in, particle.mImageName)
				>> label >> particle.mParticlePos.x >> label >> particle.mParticlePos.y;

			particle.mParticleType = static_cast<Particle_Type>(particle_type);

			return particle;
		}



	};





    class ParticleSystem : public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;




		//static functions to be called wherever will be put here
		void SpawnParticles(Particle part) {
			if (mParticleAmount >= MAX_PARTICLES) {
				return;
			}
			mParticleAmount++;
			mParticleList[mParticleAmount] = part;
		}



        float RandomFloatValue(float min, float max) {
            float random = ((float)rand()) / (float)RAND_MAX;
            float range = max - min;
            return (random * range) + min;
        }

		// Function to convert direction in degrees to Vector2D
		Vector2D DirectionToVector2D(float directionDegrees) {
			float radians = directionDegrees * (PI / 180.0f);
			Vector2D vector;
			vector.x = cos(radians);
			vector.y = sin(radians);
			return vector;
		}


		void DeleteParticle(int id) {
			mParticleList[id] = mParticleList[mParticleAmount];
			mParticleAmount--;

		}

		void ClearParticles() { mParticleAmount = 0; };


    private:
		int mParticleAmount;
		std::array<Particle,MAX_PARTICLES> mParticleList; //fixed array of particles
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H  