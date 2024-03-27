/*!
 * \file CreditsScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This c# file contains the CreditsScript class, used for Credits of the game
 * 
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class CreditsScript
    {
        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static private float total_timer;
        static private float timer;
        static private float dt;
        static private float bottomY;
        static private float topY;
        static private float offset;
        static private float lineBreak;
        static private float releasedYPos;

        static private float fadeDuration;
        static private float alpha;


        static private float gameByGroupStart;
        static private float gameByGroupDuration;
        static private float memberGroupStart; 
        static private float memberGroupEnd;
        static private float postMemberGroupWaitTime;
        static private float nextSectionStartTime;
        static private float currentTimeAfterMembers;
        static private float memberDisplayDuration;
        static private float totalMemberDisplayTime;
        static private float durationBetweenGroups;
        static private float postDisplayWaitTime;
        static private float endingGroupStartTime;

        static private List<(string name, string role)> teamMembers = new List<(string name, string role)>();
        static private List<string> tech_instructors = new List<string>();
        static private List<string> design_instructors = new List<string>();
        static private List<string> audio_instructors = new List<string>();
        static private List<string> art_instructors = new List<string>();
        static private List<string> boardOfDirectors = new List<string>();
        static private List<string> executives = new List<string>();

        static float leaves_timer;
        static float leaves_timer_set=2f;
        static int leaves_amount = 100;

        static private int back_button_entity;

        static SimpleImage button_frame;

        static Vector2D back_button_pos = new Vector2D(0, 0);
        static public void Init()
        {
            button_frame = InternalCalls.GetSpriteImage("button_frame.png");
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", button_frame, "BackFromWinScript");

            total_timer = 0f;
            timer = 0f;
            dt = InternalCalls.GetDeltaTime();
            bottomY =3.95f; // End position for the text (above the top of the screen)
            topY = 0f; // Top position for the text (below the bottom of the screen)

            //offsets
            offset = 0.05f;
            lineBreak = offset;

            teamMembers.Clear();
            tech_instructors.Clear();
            design_instructors.Clear();
            audio_instructors.Clear();
            art_instructors.Clear();
            boardOfDirectors.Clear();
            executives.Clear();

            teamMembers.Add(("Koh  Yan  Khang", "Product Manager"));
            teamMembers.Add(("Tan  Zheng  Xun", "Technical Lead"));
            teamMembers.Add(("Ee  Tze  Rong  Cheryl", "Design Lead"));
            teamMembers.Add(("Lim  Wan  Jin", "Art Lead"));
            teamMembers.Add(("Guo  Yiming", "Programmer"));
            teamMembers.Add(("Ng  De  En  Matthew", "Programmer"));
            teamMembers.Add(("Wu  Zekai", "Programmer"));
            teamMembers.Add(("Ang Qin Tian Rachel", "Artist"));

            alpha = 0f;

            fadeDuration =  1.5f;
            durationBetweenGroups = 1f;
            gameByGroupStart = 0f;
            gameByGroupDuration = 1.5f;
            memberGroupStart = gameByGroupStart + gameByGroupDuration + durationBetweenGroups;
            memberDisplayDuration = 3f;
            totalMemberDisplayTime = teamMembers.Count * (memberDisplayDuration + durationBetweenGroups);

            postMemberGroupWaitTime = 3f;
            memberGroupEnd = memberGroupStart + totalMemberDisplayTime + postMemberGroupWaitTime;
            nextSectionStartTime = memberGroupEnd;

            postDisplayWaitTime = 2f;
            endingGroupStartTime = 0f;
            tech_instructors.Add("Elie Hosry");
            tech_instructors.Add("Goh Dian Yang");
            design_instructors.Add("Lee Chee Yuan Alwyn");
            design_instructors.Add("Goh Jing Ying");
            design_instructors.Add("Danielle Fong Sum Yee");
            design_instructors.Add("Holger Liebnitz");
            audio_instructors.Add("Vuk Krakovic");
            audio_instructors.Add("Gavin Parker");
            art_instructors.Add("Keh Choon Wee");

            boardOfDirectors.Add("Chu Jason Yeu Tat");
            boardOfDirectors.Add("Michele Comair");
            boardOfDirectors.Add("Christopher Comair");
            boardOfDirectors.Add("Alexander Comair");

            executives.Add("Michael Gats");
            executives.Add("Tan Chek Ming");
            executives.Add("Prasanna Kumar Ghali");
            executives.Add("Mandy Wong");
            executives.Add("Johnny Deek");

            //InternalCalls.SetSpriteAlpha(0.95f);
        }

        static public void Update()
        {
            //zx added stuff
            InternalCalls.DrawSquare(0, 0, 7000, 7000, 0, 0, 0, 0.5f, 2);
            InternalCalls.AttachCamera(0, 0);
            leaves_timer -= InternalCalls.GetDeltaTime();
            //spawn leaves
            if (leaves_timer <= 0)
            {


                for (int i = 0; i < leaves_amount; i++)
                {
                    int col = (int)(InternalCalls.GetRandomFloat() * 2);
                    float direction = InternalCalls.GetRandomFloat() * 360;
                    float size = InternalCalls.GetRandomFloat() * 150;
                    float speed = InternalCalls.GetRandomFloat() * 800f;
                    float lifespan = InternalCalls.GetRandomFloat() * 12f;
                    InternalCalls.GameSpawnParticleExtraFramesFull((CameraScript.camera_pos.x - 2000) + (2000 * InternalCalls.GetRandomFloat()), CameraScript.camera_pos.y + 2000f,
                        direction, size, 0f, 1f,
                        0f, lifespan, speed, "ParticleLeaves.txt"
                        , col, 0, direction, 4, 1f, lifespan * CustomMath.Normalize(lifespan - 6f) / 5f, InternalCalls.GetTopLayer(), (1, 1, 1));

                }

                leaves_timer = leaves_timer_set;
            }

            //ends here

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            // Store the current position before checking mouse button state
            //releasedYPos = normalizedYPos;

            //// Check for click event
            //if (InternalCalls.MouseHeld(0))
            //{
            //    duration = 5f;
            //}
            //else {
            //    duration = 10f;
            //    topY = releasedYPos;
            //}

            Vector2D button = new Vector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y);




            if (total_timer > gameByGroupStart && total_timer <= gameByGroupStart + gameByGroupDuration)
            {
                alpha = CalculateFadeAlpha();
                InternalCalls.RenderTextFont("FRAGMENTS", "MedusaGothic_D", 0.5f, 0.55f, 15f, (1f, 1f, 1f, alpha));
                InternalCalls.RenderTextFont("By Insight", "Semplicita_Light", 0.5f, 0.5f, 18f, (1f, 1f, 1f, alpha));
            }
            else if (total_timer > gameByGroupStart + gameByGroupDuration && total_timer <= memberGroupStart)
            {
                timer = 0f; // Reset for the next group
            }

            if (total_timer > memberGroupStart && total_timer <= memberGroupStart + totalMemberDisplayTime)
            {
                float timeSinceGroupStart = total_timer - memberGroupStart;
                int memberIndex = (int)(timeSinceGroupStart / (memberDisplayDuration + durationBetweenGroups));
                float timeInCurrentSegment = timeSinceGroupStart % (memberDisplayDuration + durationBetweenGroups);

                //Check if we are in the display time(not in the gap)
                if (timeInCurrentSegment <= memberDisplayDuration && memberIndex < teamMembers.Count)
                {
                    var member = teamMembers[memberIndex];

                    float alpha = 1f; // Default to fully visible
                    if (timeInCurrentSegment <= memberDisplayDuration / 2)
                    {
                        //fade in
                        alpha = timeInCurrentSegment / (memberDisplayDuration / 2);
                    }
                    else
                    {
                        //fade out
                        alpha = 1 - ((timeInCurrentSegment - (memberDisplayDuration / 2)) / (memberDisplayDuration / 2));
                    }

                    InternalCalls.RenderTextFont(member.name, "MedusaGothic_D", 0.5f, 0.55f, 15f, (1f, 1f, 1f, alpha));
                    InternalCalls.RenderTextFont(member.role, "Semplicita_Light", 0.5f, 0.55f - lineBreak, 18f, (1f, 1f, 1f, alpha));
                }
            }
            else if (total_timer > gameByGroupStart + gameByGroupDuration && total_timer <= memberGroupStart)
            {
                timer = 0f; // Reset for the next group
            }
            currentTimeAfterMembers = total_timer - nextSectionStartTime;
            if (total_timer > nextSectionStartTime)
            {
                // Calculate the interpolation factor based on the timer after members display
                float duration = 20f; // Total duration to complete the interpolation, adjust as needed
                float t = Math.Clamp(currentTimeAfterMembers / duration, 0f, 1f);
                // Calculate the normalized Y position by interpolating between startY and endY
                float normalizedYPos = Lerp(topY, bottomY, t);


                InternalCalls.RenderTextFont("Tech Instructors", "MedusaGothic_D", 0.5f, normalizedYPos, 15f, (1f, 1f, 1f, 1f));
                float techInstructorLineBreak = 1.5f;
                foreach (string instructor in tech_instructors)
                {
                    InternalCalls.RenderTextFont(instructor, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * techInstructorLineBreak, 18f, (1f, 1f, 1f, 1f));
                    techInstructorLineBreak += 1.5f;
                }

                InternalCalls.RenderTextFont("Design Instructors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (techInstructorLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
                float designInstructorLineBreak = techInstructorLineBreak + 3f;
                foreach (string instructor in design_instructors)
                {
                    InternalCalls.RenderTextFont(instructor, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * designInstructorLineBreak, 18f, (1f, 1f, 1f, 1f));
                    designInstructorLineBreak += 1.5f;
                }

                InternalCalls.RenderTextFont("Audio Instructors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (designInstructorLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
                float audioInstructorLineBreak = designInstructorLineBreak + 3f;
                foreach (string instructor in audio_instructors)
                {
                    InternalCalls.RenderTextFont(instructor, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * audioInstructorLineBreak, 18f, (1f, 1f, 1f, 1f));
                    audioInstructorLineBreak += 1.5f;
                }

                InternalCalls.RenderTextFont("Art Instructors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (audioInstructorLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
                float artInstructorLineBreak = audioInstructorLineBreak + 3f;
                foreach (string instructor in art_instructors)
                {
                    InternalCalls.RenderTextFont(instructor, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * artInstructorLineBreak, 18f, (1f, 1f, 1f, 1f));
                    artInstructorLineBreak += 1.5f;
                }

                InternalCalls.RenderTextFont("Created  At", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (artInstructorLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
                InternalCalls.RenderTextFont("Digipen Insitute of Technology Singapore", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (artInstructorLineBreak + 3f), 18f, (1f, 1f, 1f, 1f));

                InternalCalls.RenderTextFont("President", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (artInstructorLineBreak + 6f), 15f, (1f, 1f, 1f, 1f));
                InternalCalls.RenderTextFont("Claude Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (artInstructorLineBreak + 7.5f), 18f, (1f, 1f, 1f, 1f));


                InternalCalls.RenderTextFont("Board  of  Directors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (artInstructorLineBreak + 10.5f), 15f, (1f, 1f, 1f, 1f));
                float bodLineBreak = artInstructorLineBreak + 12f;
                foreach (string bod in boardOfDirectors)
                {
                    InternalCalls.RenderTextFont(bod, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * bodLineBreak, 18f, (1f, 1f, 1f, 1f));
                    bodLineBreak += 1.5f;
                }

                InternalCalls.RenderTextFont("Executives", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (bodLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));

                float execLineBreak = bodLineBreak + 3f;
                foreach (string exec in executives)
                {
                    InternalCalls.RenderTextFont(exec, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * execLineBreak, 18f, (1f, 1f, 1f, 1f));
                    execLineBreak += 1.5f;
                }

                float specialThanksLineBreak = execLineBreak + 1.5f;

                InternalCalls.RenderTextFont("Special  Thanks", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * specialThanksLineBreak, 15f, (1f, 1f, 1f, 1f));
                specialThanksLineBreak += 1.5f;
                InternalCalls.RenderTextFont("Ian Chua Rong Bin", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * specialThanksLineBreak, 18f, (1f, 1f, 1f, 1f));
                specialThanksLineBreak += 1.5f;
                InternalCalls.RenderTextFont("www.digipen.edu", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (specialThanksLineBreak + .5f), 9f, (1f, 1f, 1f, 1f));
                InternalCalls.RenderTextFont("All content 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (specialThanksLineBreak + 1.25f), 9f, (1f, 1f, 1f, 1f));
                
                endingGroupStartTime = nextSectionStartTime + duration + postDisplayWaitTime;
                //float fadeEffectDuration = 5f; // Total duration of the fade effect
                float currentTimeInFadeEffect = total_timer - endingGroupStartTime;

                if (currentTimeInFadeEffect >= 0 && currentTimeInFadeEffect <= fadeDuration + 2f) 
                {
                    if (currentTimeInFadeEffect <= fadeDuration)
                    {
                        // Fade in
                        alpha = currentTimeInFadeEffect / (fadeDuration / 2 );
                    }
                 
                    InternalCalls.RenderTextFont("Thank you for playing", "MedusaGothic_D", 0.5f, 0.5f, 15f, (1f, 1f, 1f, alpha));
                }
                else if (currentTimeInFadeEffect > fadeDuration + 3f)
                {
                    InternalCalls.SetEntityLayer(InternalCalls.GetTopLayer(), back_button_entity);
                    InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(button.x, button.y));
                    back_button_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.5f * win_dimension.y));
                    InternalCalls.RenderTextFont("EXIT", "MedusaGothic_D", 0.5f, 0.505f, 7.5f, (1f, 1f, 1f, 1f));
                    InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);

                }
            }

            timer += dt;
            total_timer += dt;

        }

        static public void CleanUp()
        {
        }

        static private float Lerp(float start, float end, float t)
        {
            return start + (end - start) * t;
        }

        static private float CalculateFadeAlpha()
        {
            float alpha;
            if (timer <= fadeDuration / 2f)
            {
                alpha = timer / (fadeDuration / 2f); // Fade in
            }
            else
            {
                alpha = 1f - ((timer - fadeDuration / 2f) / (fadeDuration / 2f)); // Fade out
            }

            if (timer >= fadeDuration)
            {
                timer = 0f; // Reset timer for next calculation
            }

            return alpha;
        }
    }
}