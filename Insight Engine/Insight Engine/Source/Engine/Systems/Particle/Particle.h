/* Start Header **************************************************************/
/*!
 * \file Particle.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * Declaration of structs/class, systems for particles
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/
 /*                                                                   guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Engine/ECS/System.h"
#include <array>
#include <fstream>
#include <sstream>
#define MAX_PARTICLES 50000


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
		pt_texture_frames=3,
		pt_anim=4

	};

	enum Particle_Effect : int {
		effect_normal=0,
		effect_swing=1,
		effect_spin=2,
		effect_animate=3
	};

	/*!
	 * \class Color
	 * \brief Represents a color with red, green, blue, and alpha components.
	 */
	class Color {
	public:
		/*!
		 * \brief Constructor for Color class with individual color components.
		 * \param red Red component of the color.
		 * \param green Green component of the color.
		 * \param blue Blue component of the color.
		 * \param alpha Alpha component of the color.
		 */
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

	/*!
	 * \struct Particle
	 * \brief Represents a particle with various attributes.
	 */
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
		int mLayer = 1;
		bool mGravity=false;
		int mColIndex = 0;
		int mRowIndex = 0;
		int mTotalCols = 0;
		int mTotalRows = 0;
		float mRotationSpeed = 0.f;
		Vector2D mParticlePos = { 0.f,0.f };
		Particle_Effect mEffect = effect_normal;
		float mParticleEffectTimer = 0.f;
		float mParticleEffectTimerSet = 2.f;

		/*!
		 * \brief Creates and returns a new Particle object.
		 * \param velocity Initial velocity of the particle.
		 * \param size_growth Rate of size growth of the particle.
		 * \param scale Scale of the particle.
		 * \param lifespan Lifespan of the particle.
		 * \param color Color of the particle.
		 * \param alpha Alpha value of the particle.
		 * \param alpha_growth Rate of alpha value change.
		 * \param rotation Rotation angle of the particle.
		 * \param direction Direction of the particle movement.
		 * \param particle_type Type of the particle.
		 * \param image_name Name of the image associated with the particle.
		 * \param particle_pos Initial position of the particle.
		 * \return A new Particle object.
		 */
		static Particle CreateParticle(const Vec2D& velocity, float size_growth,
			const Vector2D& scale, float lifespan,
			const Color& color, float alpha, float alpha_growth,
			float rotation, float direction, Particle_Type particle_type,
			const std::string& image_name, const Vector2D& particle_pos,
			int totalcols,int totalrows,
			int curcol, int currow) {
			Particle new_particle;
			new_particle.mVelocity = velocity;
			new_particle.mSizeGrowth = size_growth;
			new_particle.mScale = scale;
			new_particle.mLifespan = lifespan;
			new_particle.mColor = color;
			new_particle.mAlpha = alpha;
			new_particle.mAlphaGrowth = alpha_growth;
			new_particle.mRotation = rotation;
			new_particle.mDirection = direction;
			new_particle.mParticleType = particle_type;
			new_particle.mImageName = image_name;
			new_particle.mParticlePos = particle_pos;
			new_particle.mTotalCols = totalcols;
			new_particle.mTotalRows = totalrows;
			new_particle.mColIndex = curcol;
			new_particle.mRowIndex = currow;

			return new_particle;
		}

		static Particle CreateParticleFrames(Particle& part, int col, int row) {
			part.mColIndex = col;
			part.mRowIndex = row;
			return part;
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
				<< "Particle Position: " << mParticlePos.x << ", " << mParticlePos.y << "\n"
				<< "Total Cols: " << mTotalCols << "\n"
				<< "Total Rows: " << mTotalRows << "\n"
				<< "Layer: " << mLayer << "\n"
				<< "Gravity: " << mLayer << "\n";
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

			// Deserialize the rows and cols for animation
			std::getline(in, label, ':'); in >> particle.mTotalCols;
			std::getline(in, label);
			std::getline(in, label, ':'); in >> particle.mTotalRows;			
			std::getline(in, label);
			std::getline(in, label, ':'); in >> particle.mLayer;			
			std::getline(in, label);
			std::getline(in, label, ':'); in >> particle.mGravity;

			return particle;
		}
	};

	/*!
	 * \class ParticleSystem
	 * \brief Manages particle effects and behaviors.
	 */
    class ParticleSystem : public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string GetName() override;
        void HandleMessage(const Message& message) override;

		//static functions to be called wherever will be put here

		/*!
		 * \brief Spawns particles in the system.
		 * \param particles Particle object to be spawned.
		 */
		void SpawnParticles(Particle particles) {
			if (mParticleAmount >= MAX_PARTICLES) {
				return;
			}
			
			mParticleList[mParticleAmount] = particles;
			mParticleAmount++;
		}
		
		/*!
		 * \brief Generates a random floating-point number between min and max.
		 * \param min Minimum value.
		 * \param max Maximum value.
		 * \return A random float between min and max.
		 */
        float RandomFloatValue(float min, float max) {
            float random = ((float)rand()) / (float)RAND_MAX;
            float range = max - min;
            return (random * range) + min;
        }

		/*!
		 * \brief Converts a direction in degrees to a Vector2D.
		 * \param direction_degrees Direction in degrees.
		 * \return A Vector2D representing the direction.
		 */
		Vector2D DirectionToVector2D(float direction_degrees) {
			float radians = direction_degrees * (PI / 180.0f);
			Vector2D vector;
			vector.x = cos(radians);
			vector.y = sin(radians);
			return vector;
		}

		/*!
		 * \brief Deletes a particle from the system by its ID.
		 * \param id ID of the particle to be deleted.
		 */
		void DeleteParticle(int id) {
			mParticleList[id] = mParticleList[mParticleAmount];
			mParticleAmount--;

		}

		/*!
		 * \brief Clears all particles.
		 */
		void ClearParticles() { mParticleAmount = 0; };


    private:
		int mParticleAmount=0;
		std::array<Particle,MAX_PARTICLES> mParticleList; //fixed array of particles

    };


}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_PARTICLE_H  