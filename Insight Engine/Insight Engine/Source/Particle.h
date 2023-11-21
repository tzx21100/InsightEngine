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
#define MAX_PARTICLES 20000


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
			out << "Red: " << R << "\n"
				<< "Green: " << G << "\n"
				<< "Blue: " << B << "\n"
				<< "Alpha: " << A << "\n";
			return out.str();
		}


		// Static method to deserialize a string to a Color object
		static Color Deserialize(const std::string& data) {
			std::istringstream in(data);
			float r, g, b, a;
			std::string label;

			// Extract each color component with its label
			in >> label >> r; // Reads "Red: <value>"
			in >> label >> g; // Reads "Green: <value>"
			in >> label >> b; // Reads "Blue: <value>"
			in >> label >> a; // Reads "Alpha: <value>"

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
		int mParticleType = pt_square;
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

			// Deserialize velocity
			std::getline(in, label, ':'); in >> particle.mVelocity.x;
			in.ignore(2); // Ignore the comma and space
			in >> particle.mVelocity.y;
			std::getline(in, label); // Finish the line

			// Deserialize size growth
			std::getline(in, label, ':'); in >> particle.mSizeGrowth;
			std::getline(in, label);

			// Deserialize scale
			std::getline(in, label, ':'); in >> particle.mScale.x;
			in.ignore(2); // Ignore the comma and space
			in >> particle.mScale.y;
			std::getline(in, label);

			// Deserialize lifespan
			std::getline(in, label, ':'); in >> particle.mLifespan;
			std::getline(in, label);

			// Deserialize color
			std::getline(in, label); // Skip "Color" line
			std::string colorData;
			for (int i = 0; i < 4; ++i) { // Assuming 4 lines for R, G, B, A
				std::getline(in, label);
				colorData += label + "\n";
			}
			particle.mColor = Color::Deserialize(colorData);

			// Deserialize alpha
			std::getline(in, label, ':'); in >> particle.mAlpha;
			std::getline(in, label);

			// Deserialize alpha growth
			std::getline(in, label, ':'); in >> particle.mAlphaGrowth;
			std::getline(in, label);

			// Deserialize rotation
			std::getline(in, label, ':'); in >> particle.mRotation;
			std::getline(in, label);

			// Deserialize direction
			std::getline(in, label, ':'); in >> particle.mDirection;
			std::getline(in, label);

			// Deserialize particle type
			std::getline(in, label, ':'); in >> particle.mParticleType;
			std::getline(in, label);

			// Deserialize image name
			std::getline(in, label, ':');
			std::getline(in, particle.mImageName);
			if (particle.mImageName.size() > 1) { // Check if the image name is not empty
				particle.mImageName = particle.mImageName.substr(1); // Remove leading space
			}

			// Deserialize particle position
			std::getline(in, label, ':'); in >> particle.mParticlePos.x;
			in.ignore(2); // Ignore the comma and space
			in >> particle.mParticlePos.y;

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
			
			mParticleList[mParticleAmount] = part;
			mParticleAmount++;
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
		int mParticleAmount=0;
		std::array<Particle,MAX_PARTICLES> mParticleList; //fixed array of particles
    };
}


#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H  