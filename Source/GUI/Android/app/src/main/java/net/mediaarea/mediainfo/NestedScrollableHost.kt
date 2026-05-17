package net.mediaarea.mediainfo

import android.content.Context
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import android.view.ViewConfiguration
import android.widget.FrameLayout
import androidx.viewpager2.widget.ViewPager2
import kotlin.math.abs
import kotlin.math.sign

class NestedScrollableHost @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null
) : FrameLayout(context, attrs) {

    private companion object {
        const val HORIZONTAL_DOMINANCE_RATIO = 1.2f // 1.0f, very sensitive, 1.2f balanced, 1.5f strict
    }

    private enum class Owner {
        NONE,
        PARENT,
        CHILD
    }

    private var initialX = 0f
    private var initialY = 0f
    private var gestureOwner: Owner = Owner.NONE
    private val touchSlop = ViewConfiguration.get(context).scaledTouchSlop

    private val parentViewPager: ViewPager2?
        get() {
            var current: View? = parent as? View
            while (current != null && current !is ViewPager2) {
                current = current.parent as? View
            }
            return current
        }

    override fun onInterceptTouchEvent(event: MotionEvent): Boolean {
        handleIntercept(event)
        return super.onInterceptTouchEvent(event)
    }

    private fun handleIntercept(event: MotionEvent) {
        parentViewPager ?: return
        val child = getChildAt(0) ?: return

        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                initialX = event.x
                initialY = event.y
                gestureOwner = Owner.NONE
                parent.requestDisallowInterceptTouchEvent(true)
            }

            MotionEvent.ACTION_MOVE -> {
                val dx = event.x - initialX
                val dy = event.y - initialY
                val absDx = abs(dx)
                val absDy = abs(dy)

                if (absDx <= touchSlop && absDy <= touchSlop) {
                    return
                }

                val isHorizontalGesture = absDx > absDy * HORIZONTAL_DOMINANCE_RATIO
                if (!isHorizontalGesture) {
                    gestureOwner = Owner.CHILD
                    parent.requestDisallowInterceptTouchEvent(true)
                    return
                }

                val direction = -dx.sign.toInt()
                if (direction == 0) {
                    return
                }

                if (gestureOwner == Owner.NONE) {
                    gestureOwner = if (child.canScrollHorizontally(direction)) Owner.CHILD else Owner.PARENT
                }

                parent.requestDisallowInterceptTouchEvent(gestureOwner == Owner.CHILD)
            }

            MotionEvent.ACTION_UP,
            MotionEvent.ACTION_CANCEL -> {
                gestureOwner = Owner.NONE
            }
        }
    }
}
